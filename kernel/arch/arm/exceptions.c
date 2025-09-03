/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

/**
 * \file    arch/arm/exceptions.c
 * \author  POK team
 * \brief   ARM Cortex-M exception handling
 */

/* POK system headers */
#include <errno.h>

/* POK core headers */
#include <core/debug.h>
#include <core/partition.h>
#include <core/sched.h>
/* Architecture-specific headers */
#include "arch.h"
#include "mpu.h"
#include "nvic.h"
#include "stm32f4/peripherals.h"

/* STM32F4 USART1 registers for non-blocking fault output */
#define USART1_SR (*((volatile uint32_t *)(USART1_BASE + 0x00)))
#define USART1_DR (*((volatile uint32_t *)(USART1_BASE + 0x04)))
#define USART_SR_TXE (1 << 7) /* Transmit data register empty */

/* Non-blocking fault output functions */
static inline void fault_putc(char c) {
  /* Try to output character without blocking */
  if (USART1_SR & USART_SR_TXE) {
    USART1_DR = c;
  }
}

static void fault_puts(const char *s) {
  while (*s) {
    fault_putc(*s++);
    /* Best-effort only; no delay to avoid prolonging fault handling */
  }
}

static void fault_put_hex(uint32_t value) {
  const char hex_chars[] = "0123456789ABCDEF";
  fault_puts("0x");
  for (int i = 28; i >= 0; i -= 4) {
    fault_putc(hex_chars[(value >> i) & 0xF]);
  }
}

static void fault_put_dec(uint32_t value) {
  char buf[10];
  int i = 0;

  if (value == 0) {
    fault_putc('0');
    return;
  }

  while (value > 0) {
    buf[i++] = '0' + (value % 10);
    value /= 10;
  }

  /* Print digits in reverse order */
  while (i > 0) {
    fault_putc(buf[--i]);
  }
}

/* CFSR (Configurable Fault Status Register) bits */
#define SCB_CFSR (*((volatile uint32_t *)(SCB_BASE + 0x28)))
#define CFSR_MMARVALID (1 << 7)  /* MemManage Fault Address Register valid */
#define CFSR_BFARVALID (1 << 15) /* Bus Fault Address Register valid */

/* Forward declarations for the actual handlers */
static void MemManage_Handler_C(uint32_t *frame);
static void BusFault_Handler_C(uint32_t *frame);
static void UsageFault_Handler_C(uint32_t *frame);
static void HardFault_Handler_C(uint32_t *frame);

/*
 * Macro to generate naked fault handler wrappers
 * Determines correct stack pointer (MSP vs PSP) based on EXC_RETURN
 */
#define DEFINE_FAULT_HANDLER_WRAPPER(handler_name, c_handler_name)             \
  void __attribute__((naked)) handler_name(void) {                             \
    __asm volatile(                                                            \
        "tst lr, #4                 \n" /* Test EXC_RETURN[2] */               \
        "ite eq                     \n" /* If-Then-Else */                     \
        "mrseq r0, msp              \n" /* If EXC_RETURN[2]==0, use MSP */     \
        "mrsne r0, psp              \n" /* If EXC_RETURN[2]==1, use PSP */     \
        "b " #c_handler_name                                                   \
        "      \n" /* Call C handler with correct frame */                     \
        ::                                                                     \
            : "r0", "memory");                                                 \
  }

DEFINE_FAULT_HANDLER_WRAPPER(MemManage_Handler, MemManage_Handler_C)

/*
 * Memory Management Fault Handler - C implementation
 * Handles MPU violations and other memory management faults
 */
static void MemManage_Handler_C(uint32_t *frame) {
  uint32_t fault_addr = 0;
  uint8_t partition_id;
  uint32_t cfsr;

  if (frame == NULL) {
    /* Cannot recover from null frame, halt system */
    __disable_irq(); /* Prevent livelock or nested faults */
    while (1) {
      __asm volatile("wfi");
    }
  }

  /* Read CFSR to check fault status */
  cfsr = SCB_CFSR;

  /* Get faulting address from MemManage Fault Address Register if valid */
  if (cfsr & CFSR_MMARVALID) {
    fault_addr = *((volatile uint32_t *)(SCB_BASE + 0x34)); /* MMFAR */
  }

  /* Clear MemManage fault flags in CFSR - write back only set bits */
  SCB_CFSR = cfsr & ARM_CFSR_MMFSR_MASK; /* Clear only set MMFSR bits */

  /* Get current partition */
  extern uint8_t pok_current_partition;
  partition_id = pok_current_partition;

#ifdef POK_NEEDS_DEBUG
  if (cfsr & CFSR_MMARVALID) {
    fault_puts("MemManage fault in partition ");
    fault_put_dec(partition_id);
    fault_puts(" at address ");
    fault_put_hex(fault_addr);
    fault_puts("\n");
  } else {
    fault_puts("MemManage fault in partition ");
    fault_put_dec(partition_id);
    fault_puts(" (address not available)\n");
  }
  fault_puts("PC: ");
  fault_put_hex(frame[6]);
  fault_puts(", LR: ");
  fault_put_hex(frame[5]);
  fault_puts(", CFSR: ");
  fault_put_hex(cfsr);
  fault_puts("\n");
#endif

  /* For safety-critical systems, halt immediately on memory faults
   * instead of attempting complex recovery from fault context */
#ifdef POK_NEEDS_DEBUG
  fault_puts("FATAL: Memory protection violation in partition ");
  fault_put_dec(partition_id);
  fault_puts(" - System halted for safety\n");
#endif

  /* Halt the system - safer than attempting partition recovery from fault
   * handler */
  __disable_irq(); /* Prevent livelock or nested faults */
  while (1) {
    __asm volatile("wfi");
  }
}

DEFINE_FAULT_HANDLER_WRAPPER(BusFault_Handler, BusFault_Handler_C)

/*
 * Bus Fault Handler - C implementation
 * Handles bus errors and invalid memory accesses
 */
static void BusFault_Handler_C(uint32_t *frame) {
  uint32_t fault_addr = 0;
  uint8_t partition_id;
  uint32_t cfsr;

  if (frame == NULL) {
    while (1) {
      __asm volatile("wfi");
    }
  }

  /* Read CFSR to check fault status */
  cfsr = SCB_CFSR;

  /* Get faulting address from Bus Fault Address Register if valid */
  if (cfsr & CFSR_BFARVALID) {
    fault_addr = *((volatile uint32_t *)(SCB_BASE + 0x38)); /* BFAR */
  }

  /* Clear Bus fault flags in CFSR - write 1 to clear */
  SCB_CFSR = ARM_CFSR_BFSR_MASK; /* Clear all BFSR bits */

  extern uint8_t pok_current_partition;
  partition_id = pok_current_partition;

#ifdef POK_NEEDS_DEBUG
  if (cfsr & CFSR_BFARVALID) {
    fault_puts("BusFault in partition ");
    fault_put_dec(partition_id);
    fault_puts(" at address ");
    fault_put_hex(fault_addr);
    fault_puts("\n");
  } else {
    fault_puts("BusFault in partition ");
    fault_put_dec(partition_id);
    fault_puts(" (address not available)\n");
  }
  fault_puts("PC: ");
  fault_put_hex(frame[6]);
  fault_puts(", LR: ");
  fault_put_hex(frame[5]);
  fault_puts(", CFSR: ");
  fault_put_hex(cfsr);
  fault_puts("\n");
#endif

  /* Halt system immediately - safer than partition recovery from fault context
   */
#ifdef POK_NEEDS_DEBUG
  fault_puts("FATAL: Bus fault recovery disabled - System halted for safety\n");
#endif
  __disable_irq(); /* Prevent livelock or nested faults */
  while (1) {
    __asm volatile("wfi");
  }
}

DEFINE_FAULT_HANDLER_WRAPPER(UsageFault_Handler, UsageFault_Handler_C)

/*
 * Usage Fault Handler - C implementation
 * Handles undefined instruction, unaligned access, etc.
 */
static void UsageFault_Handler_C(uint32_t *frame) {
  uint8_t partition_id;

  if (frame == NULL) {
    while (1) {
      __asm volatile("wfi");
    }
  }

  extern uint8_t pok_current_partition;
  partition_id = pok_current_partition;

  /* Clear Usage fault flags in CFSR - write 1 to clear */
  SCB_CFSR = ARM_CFSR_UFSR_MASK; /* Clear all UFSR bits */

#ifdef POK_NEEDS_DEBUG
  fault_puts("UsageFault in partition ");
  fault_put_dec(partition_id);
  fault_puts("\n");
  fault_puts("PC: ");
  fault_put_hex(frame[6]);
  fault_puts(", LR: ");
  fault_put_hex(frame[5]);
  fault_puts("\n");
#endif

  /* Halt system immediately - safer than partition recovery from fault context
   */
#ifdef POK_NEEDS_DEBUG
  fault_puts("FATAL: Usage fault in partition ");
  fault_put_dec(partition_id);
  fault_puts(" - System halted for safety\n");
#endif
  __disable_irq(); /* Prevent livelock or nested faults */
  while (1) {
    __asm volatile("wfi");
  }
}

/*
 * Hard Fault Handler
 * Last resort fault handler
 */
DEFINE_FAULT_HANDLER_WRAPPER(HardFault_Handler, HardFault_Handler_C)

/*
 * Hard Fault Handler - C implementation
 */
static void HardFault_Handler_C(uint32_t *frame) {
  uint8_t partition_id;

  if (frame == NULL) {
    while (1) {
      __asm volatile("wfi");
    }
  }

  extern uint8_t pok_current_partition;
  partition_id = pok_current_partition;

#ifdef POK_NEEDS_DEBUG
  fault_puts("HardFault in partition ");
  fault_put_dec(partition_id);
  fault_puts("\n");
  fault_puts("PC: ");
  fault_put_hex(frame[6]);
  fault_puts(", LR: ");
  fault_put_hex(frame[5]);
  fault_puts(", PSR: ");
  fault_put_hex(frame[7]);
  fault_puts("\n");
  fault_puts("r0: ");
  fault_put_hex(frame[0]);
  fault_puts(", r1: ");
  fault_put_hex(frame[1]);
  fault_puts(", r2: ");
  fault_put_hex(frame[2]);
  fault_puts(", r3: ");
  fault_put_hex(frame[3]);
  fault_puts("\n");
#endif

  /* Halt system immediately - HardFault indicates severe system error */
#ifdef POK_NEEDS_DEBUG
  fault_puts(
      "FATAL: Hard fault recovery disabled - System halted for safety\n");
#endif
  __disable_irq(); /* Prevent livelock or nested faults */
  while (1) {
    __asm volatile("wfi");
  }
}
