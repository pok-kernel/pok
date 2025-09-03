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
 * \file    arch/arm/arch.c
 * \author  POK team
 * \brief   Provides generic architecture interface for ARM Cortex-M
 * architecture
 */

/* POK system headers */
#include <errno.h>

/* POK core headers */
#include <arch.h>
#include <core/partition.h>

/* Architecture-specific headers */
#include "mpu.h"
#include "nvic.h"

/* Stack address calculation constants */
#define POK_STACK_GUARD_BYTES 8 /* Guard offset for stack calculations */

extern pok_ret_t pok_arch_space_init(void);

pok_ret_t pok_arch_init() {
  pok_ret_t ret;

  ret = pok_mpu_init();
  if (ret != POK_ERRNO_OK) {
    return ret;
  }

  ret = pok_nvic_init();
  if (ret != POK_ERRNO_OK) {
    /* Cleanup: disable MPU on NVIC init failure */
    pok_mpu_disable();
    return ret;
  }

  ret = pok_arch_space_init();
  if (ret != POK_ERRNO_OK) {
    /* Cleanup: disable MPU on space init failure */
    /* Note: NVIC cleanup not needed as it doesn't maintain state */
    pok_mpu_disable();
    return ret;
  }

  return POK_ERRNO_OK;
}

/* PRIMASK helper functions for improved interrupt state management */
static inline uint32_t pok_arch_primask_read(void) {
  uint32_t primask;
  __asm volatile("mrs %0, PRIMASK" : "=r"(primask)::"memory");
  return primask;
}

static inline void pok_arch_primask_write(uint32_t primask) {
  __asm volatile("msr PRIMASK, %0" ::"r"(primask) : "memory");
  __asm volatile("isb" ::: "memory");
}

/* Enhanced versions that preserve interrupt state */
pok_ret_t pok_arch_preempt_disable_save(uint32_t *prev_state) {
  uint32_t primask = pok_arch_primask_read();
  if (prev_state) {
    *prev_state = primask;
  }
  __asm volatile("cpsid i" ::: "memory");
  __asm volatile("dsb" ::: "memory");
  __asm volatile("isb" ::: "memory");
  return POK_ERRNO_OK;
}

pok_ret_t pok_arch_preempt_restore(uint32_t prev_state) {
  /* Only restore if previously enabled */
  if ((prev_state & 0x1u) == 0) {
    __asm volatile("cpsie i" ::: "memory");
    __asm volatile("isb" ::: "memory");
  }
  return POK_ERRNO_OK;
}

/* Legacy functions maintained for backward compatibility */
pok_ret_t pok_arch_preempt_disable() {
  __asm volatile("cpsid i" : : : "memory");
  __asm volatile("dsb" : : : "memory");
  __asm volatile("isb" : : : "memory");
  return POK_ERRNO_OK;
}

pok_ret_t pok_arch_preempt_enable() {
  __asm volatile("cpsie i" : : : "memory");
  __asm volatile("isb" : : : "memory");
  return POK_ERRNO_OK;
}

void pok_arch_idle() __attribute__((noreturn));

void pok_arch_idle() {
  while (1) {
    __asm volatile("wfi");
  }
}

pok_ret_t pok_arch_event_register(uint8_t vector, void (*handler)(void)) {
  return (pok_nvic_set_handler(vector, handler));
}

/**
 * Calculate stack address for a thread in a partition
 *
 * @param partition_id Partition ID (must be < POK_CONFIG_NB_PARTITIONS)
 * @param local_thread_id Local thread ID within partition
 * @return Stack address or 0 on invalid input
 */
uint32_t pok_thread_stack_addr(const uint8_t partition_id,
                               const uint32_t local_thread_id) {
  /* Validate partition_id is within valid range */
  if (partition_id >= POK_CONFIG_NB_PARTITIONS) {
    return 0; /* Invalid partition ID */
  }

  /* Check local_thread_id bounds to prevent overflow */
  uint32_t partition_size = pok_partitions[partition_id].size;
  /* Account for guard bytes in max threads calculation */
  uint32_t effective_stack_size = POK_USER_STACK_SIZE + POK_STACK_GUARD_BYTES;
  uint32_t max_threads = partition_size / effective_stack_size;
  if (local_thread_id >= max_threads) {
    return 0; /* Thread ID too large for partition */
  }

  /* Use 64-bit arithmetic to prevent overflow - cast before multiplication */
  uint64_t stack_offset_64 =
      (uint64_t)local_thread_id * (uint64_t)effective_stack_size +
      POK_STACK_GUARD_BYTES;
  if (stack_offset_64 >= partition_size) {
    return 0; /* Stack offset exceeds partition size */
  }

  uint32_t stack_offset = (uint32_t)stack_offset_64;

  /* Validate that both stack start and end are within partition bounds */
  if ((stack_offset + POK_USER_STACK_SIZE) > partition_size) {
    return 0; /* Invalid stack address - stack extends beyond partition */
  }

  /* Calculate and return 8-byte aligned stack address */
  uint32_t partition_base = pok_partitions[partition_id].base_addr;
  uint32_t stack_addr = partition_base + partition_size - stack_offset;

  /* Ensure 8-byte stack pointer alignment for ARM Cortex-M */
  return stack_addr & ~7;
}

/**
 * Trigger a division by zero error for testing or error handling
 *
 * This function intentionally causes a division by zero to test UsageFault
 * handling when DIV_0_TRP is enabled in the SCB Configuration Control Register.
 * Used for testing exception handling or as a controlled failure mechanism.
 *
 * @note This function never returns
 */
__attribute__((noreturn)) void pok_division_by_zero_error(void) {
  /* Force a division by zero to trigger UsageFault (when DIV_0_TRP enabled) */
  volatile int zero = 0;
  volatile int result = 42 / zero;
  (void)result;

  while (1) {
    __asm volatile("wfi");
  }
}
