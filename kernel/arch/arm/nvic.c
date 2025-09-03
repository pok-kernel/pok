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
 * \file    arch/arm/nvic.c
 * \author  POK team
 * \brief   ARM Cortex-M NVIC (Nested Vectored Interrupt Controller)
 * implementation
 */

/* POK system headers */
#include <errno.h>
#include <libc.h>

/* Architecture-specific headers */
#include "arch.h"
#include "cortex_m_config.h"
#include "nvic.h"
#include "stm32f4/peripherals.h"

/* External vector table (defined in startup code) */
/* Note: We get the ROM vector table location from VTOR instead of assuming
 * a fixed location, making this more robust across different memory layouts */

/* RAM-based vector table for runtime handler updates */
#define NVIC_VECTOR_COUNT CORTEX_M_NVIC_VECTOR_COUNT
#define NVIC_VECTOR_TABLE_SIZE CORTEX_M_NVIC_VECTOR_TABLE_SIZE
#define NVIC_VECTOR_TABLE_ALIGNMENT CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT
static vector_table_entry_t ram_vector_table[NVIC_VECTOR_COUNT]
    __attribute__((aligned(NVIC_VECTOR_TABLE_ALIGNMENT)));
static uint8_t vector_table_relocated = 0;

/* Default handlers */
static void pok_nvic_default_handler(void) {
  /* Default handler - log error and halt system safely
   * Avoid WFI as it may create unrecoverable state if no other interrupts occur
   */
#ifdef POK_NEEDS_DEBUG
  printf("FATAL: Unhandled interrupt/exception occurred\n");
#endif

  /* Disable interrupts and halt */
  __disable_irq();
  while (1) {
    /* Busy wait instead of WFI to ensure system remains debuggable */
    __asm volatile("nop");
  }
}

/**
 * Relocate vector table from FLASH to RAM for runtime handler updates
 */
static pok_ret_t pok_nvic_relocate_vector_table(void) {
  if (vector_table_relocated) {
    return POK_ERRNO_OK; /* Already relocated */
  }

  /* Get current ROM vector table location from VTOR register
   * This makes the code more robust across different memory layouts */
  uint32_t rom_table_addr = SCB_VTOR;

  /* Validate ROM table address is in Flash region and entire table fits */
  if (rom_table_addr < STM32F4_FLASH_BASE ||
      rom_table_addr >= (STM32F4_FLASH_BASE + STM32F4_FLASH_SIZE)) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: ROM vector table at invalid address: 0x%x (outside Flash "
           "region 0x%x-0x%x)\n",
           rom_table_addr, STM32F4_FLASH_BASE,
           STM32F4_FLASH_BASE + STM32F4_FLASH_SIZE - 1);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Ensure full vector table fits in Flash using overflow-safe math */
  uint32_t flash_end = STM32F4_FLASH_BASE + STM32F4_FLASH_SIZE;
  uint32_t table_end = rom_table_addr + NVIC_VECTOR_TABLE_SIZE;

  /* Check for overflow in table_end calculation */
  if (table_end < rom_table_addr || table_end > flash_end) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: ROM vector table (0x%x + %u bytes) extends beyond Flash "
           "region (end: 0x%x)\n",
           rom_table_addr, NVIC_VECTOR_TABLE_SIZE, flash_end);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Validate vector table alignment */
  if ((rom_table_addr & (NVIC_VECTOR_TABLE_ALIGNMENT - 1)) != 0) {
#ifdef POK_NEEDS_DEBUG
    printf(
        "ERROR: ROM vector table misaligned: 0x%x (must be %d-byte aligned)\n",
        rom_table_addr, NVIC_VECTOR_TABLE_ALIGNMENT);
#endif
    return POK_ERRNO_EINVAL;
  }

  vector_table_entry_t *rom_vector_table =
      (vector_table_entry_t *)rom_table_addr;

  /* Copy ROM vector table to RAM */
  for (int i = 0; i < NVIC_VECTOR_COUNT; i++) {
    ram_vector_table[i] = rom_vector_table[i];
  }

  /* Update VTOR register to point to RAM vector table */
  uint32_t ram_table_addr = (uint32_t)ram_vector_table;

  /* Validate alignment (must be next power of 2 of table size) */
  if (ram_table_addr & (NVIC_VECTOR_TABLE_ALIGNMENT - 1)) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: RAM vector table not properly aligned: 0x%x (required: %d "
           "bytes)\n",
           ram_table_addr, NVIC_VECTOR_TABLE_ALIGNMENT);
#endif
    return POK_ERRNO_EFAULT;
  }

  SCB_VTOR = ram_table_addr;
  vector_table_relocated = 1;

#ifdef POK_NEEDS_DEBUG
  printf("Vector table relocated to RAM at 0x%x\n", ram_table_addr);
#endif

  __asm volatile("dsb" ::: "memory");
  __asm volatile("isb");
  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_init(void) {
  pok_ret_t ret;

  /* Relocate vector table to RAM for runtime handler updates */
  ret = pok_nvic_relocate_vector_table();
  if (ret != POK_ERRNO_OK) {
    return (ret);
  }

  /* Enable division-by-zero trap to trigger UsageFault */
  SCB_CCR |= SCB_CCR_DIV_0_TRP;

  /* Enable memory management, bus fault, and usage fault exceptions */
  SCB_SHCSR |=
      SCB_SHCSR_MEMFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_USGFAULTENA;

  /* Set fault handlers to high priority for proper error handling */
  pok_nvic_set_priority(EXCEPTION_MEMMANAGE, NVIC_PRIORITY_HIGH);
  pok_nvic_set_priority(EXCEPTION_BUSFAULT, NVIC_PRIORITY_HIGH);
  pok_nvic_set_priority(EXCEPTION_USAGEFAULT, NVIC_PRIORITY_HIGH);

  /* Set PendSV and SysTick to lowest priority for context switching */
  pok_nvic_set_priority(EXCEPTION_PENDSV, NVIC_PRIORITY_LOWEST);
  pok_nvic_set_priority(EXCEPTION_SYSTICK, NVIC_PRIORITY_LOWEST);

  /* Clear all pending interrupts based on actual IRQ count
   * Each ICPR register handles 32 IRQs, so calculate number of registers needed
   * STM32F4 has 82 external IRQs, requiring 3 ICPR registers (0-2) */
  const int external_irqs =
      CORTEX_M_NVIC_VECTOR_COUNT - 16; /* Subtract 16 system vectors */
  const int icpr_regs_needed =
      (external_irqs + 31) / 32; /* Round up to next register */

  for (int i = 0; i < icpr_regs_needed; i++) {
    NVIC_ICPR[i] = 0xFFFFFFFFU;
  }

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_set_handler(uint8_t irq, void (*handler)(void)) {
  if (irq >= NVIC_VECTOR_COUNT) {
    return POK_ERRNO_EINVAL;
  }

  /* Prevent overwriting critical system vectors */
  if (irq == 0 || irq == 1) {
    /* Vector 0: Initial Stack Pointer (MSP), Vector 1: Reset Handler */
    return POK_ERRNO_EINVAL; /* Cannot modify critical system vectors */
  }

  /* Ensure vector table has been relocated to RAM */
  if (!vector_table_relocated) {
    pok_ret_t ret = pok_nvic_relocate_vector_table();
    if (ret != POK_ERRNO_OK) {
      return (ret);
    }
  }

  /* Disable interrupts during handler update to prevent race conditions */
  uint8_t irq_was_enabled = 0;
  uint32_t primask_state = 0;

  if (irq >= EXCEPTION_IRQ0) {
    /* External IRQ - disable specific IRQ */
    uint8_t external_irq = irq - EXCEPTION_IRQ0;
    uint32_t reg_idx = external_irq / 32;
    uint32_t bit_pos = external_irq % 32;
    if (NVIC_ISER[reg_idx] & (1U << bit_pos)) {
      irq_was_enabled = 1;
      NVIC_ICER[reg_idx] = (1U << bit_pos); /* Disable IRQ */
    }
  } else if (irq >= 2 && irq <= 15) {
    /* System exception - disable all interrupts to prevent race */
    primask_state = __get_PRIMASK();
    __disable_irq();
  }

  /* Set handler in RAM vector table */
  if (handler == NULL) {
    ram_vector_table[irq] = pok_nvic_default_handler;
  } else {
    ram_vector_table[irq] = handler;
  }

  /* Data Synchronization Barrier to ensure vector table update completes */
  __asm volatile("dsb" : : : "memory");
  __asm volatile("isb");

  /* Re-enable interrupts */
  if (irq >= EXCEPTION_IRQ0) {
    /* External IRQ - re-enable specific IRQ if it was enabled */
    if (irq_was_enabled) {
      uint8_t external_irq = irq - EXCEPTION_IRQ0;
      uint32_t reg_idx = external_irq / 32;
      uint32_t bit_pos = external_irq % 32;
      NVIC_ISER[reg_idx] = (1U << bit_pos);
    }
  } else if (irq >= 2 && irq <= 15) {
    /* System exception - restore global interrupt state */
    __set_PRIMASK(primask_state);
  }

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_enable_irq(uint8_t irq) {
  if (irq < EXCEPTION_IRQ0 || irq >= NVIC_VECTOR_COUNT) {
    return POK_ERRNO_EINVAL;
  }

  uint8_t external_irq = irq - EXCEPTION_IRQ0;
  uint32_t reg_idx = external_irq / 32;
  uint32_t bit_pos = external_irq % 32;

  NVIC_ISER[reg_idx] = (1U << bit_pos);

  /* Data Synchronization Barrier to ensure register write completes */
  __asm volatile("dsb" : : : "memory");

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_disable_irq(uint8_t irq) {
  if (irq < EXCEPTION_IRQ0 || irq >= NVIC_VECTOR_COUNT) {
    return POK_ERRNO_EINVAL;
  }

  uint8_t external_irq = irq - EXCEPTION_IRQ0;
  uint32_t reg_idx = external_irq / 32;
  uint32_t bit_pos = external_irq % 32;

  NVIC_ICER[reg_idx] = (1U << bit_pos);

  /* Data Synchronization Barrier to ensure register write completes */
  __asm volatile("dsb" : : : "memory");

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_set_priority(uint8_t irq, uint8_t priority) {
  if (irq >= NVIC_VECTOR_COUNT || priority > NVIC_PRIORITY_LOWEST) {
    return POK_ERRNO_EINVAL;
  }

  if (irq < EXCEPTION_IRQ0) {
    /* System exception priority */
    volatile uint32_t *shpr_reg;
    uint8_t reg_offset;

    if (irq >= 4 && irq <= 6) {
      /* MemManage (4), BusFault (5), UsageFault (6) */
      shpr_reg = (volatile uint32_t *)&SCB_SHPR1;
      reg_offset = (irq - 4) * 8;
    } else if (irq == 11) {
      /* SVCall (11) only */
      shpr_reg = (volatile uint32_t *)&SCB_SHPR2;
      reg_offset = 24; /* SVCall is at bits [31:24] of SHPR2 */
    } else if (irq == 12 || (irq >= 14 && irq <= 15)) {
      /* DebugMon (12), PendSV (14), SysTick (15) */
      shpr_reg = (volatile uint32_t *)&SCB_SHPR3;
      if (irq == 12) {
        reg_offset = 0; /* DebugMon is at bits [7:0] of SHPR3 */
      } else if (irq == 14) {
        reg_offset = 16; /* PendSV is at bits [23:16] of SHPR3 */
      } else {           /* irq == 15 */
        reg_offset = 24; /* SysTick is at bits [31:24] of SHPR3 */
      }
    } else {
      return POK_ERRNO_EINVAL;
    }

    /* Validate offset is within 32-bit register and properly aligned for 8-bit
     * priority field */
    if (reg_offset > 24 || (reg_offset & 7) != 0) {
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: Invalid priority register offset %d for IRQ %d\n",
             reg_offset, irq);
#endif
      return POK_ERRNO_EINVAL;
    }

    uint32_t mask = ~(ARM_PRIORITY_MASK << reg_offset);
    *shpr_reg = (*shpr_reg & mask) | ((priority << 4) << reg_offset);
  } else {
    /* External interrupt priority */
    uint8_t external_irq = irq - EXCEPTION_IRQ0;
    NVIC_IPR[external_irq] = priority << 4;
  }

  /* Data Synchronization Barrier to ensure priority register write completes */
  __asm volatile("dsb" : : : "memory");

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_clear_pending(uint8_t irq) {
  if (irq < EXCEPTION_IRQ0 || irq >= NVIC_VECTOR_COUNT) {
    return POK_ERRNO_EINVAL;
  }

  uint8_t external_irq = irq - EXCEPTION_IRQ0;
  uint32_t reg_idx = external_irq / 32;
  uint32_t bit_pos = external_irq % 32;

  NVIC_ICPR[reg_idx] = (1U << bit_pos);

  return POK_ERRNO_OK;
}

pok_ret_t pok_nvic_set_vector_table(uint32_t offset) {
  /* Vector table must be aligned to next power of 2 of table size */
  if (offset & (NVIC_VECTOR_TABLE_ALIGNMENT - 1)) {
    return POK_ERRNO_EINVAL;
  }

  SCB_VTOR = offset;
  __asm volatile("dsb" : : : "memory");
  __asm volatile("isb");
  return POK_ERRNO_OK;
}

/* Default exception handlers */
void NMI_Handler(void) { pok_nvic_default_handler(); }

void DebugMon_Handler(void) { pok_nvic_default_handler(); }
