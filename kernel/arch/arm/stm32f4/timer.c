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
 * \file    arch/arm/stm32f4/timer.c
 * \author  POK team
 * \brief   STM32F4 system timer using SysTick
 */

#include "../../../../test_deployment.h"
#include "../nvic.h"
#include "clock_config.h"
#include "peripherals.h"
#include <core/sched.h>
#include <core/time.h>
#include <errno.h>
#include <libc.h>

/* SysTick registers */
/* SYSTICK_BASE now defined in peripherals.h */
#define SYSTICK_CSR (*((volatile uint32_t *)(SYSTICK_BASE + 0x00)))
#define SYSTICK_RVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x04)))
#define SYSTICK_CVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x08)))

/* SysTick Control and Status Register bits */
#define SYSTICK_CSR_ENABLE (1 << 0)
#define SYSTICK_CSR_TICKINT (1 << 1)
#define SYSTICK_CSR_CLKSOURCE (1 << 2)

/* Timer tick frequency (100 Hz = 10ms ticks) */
#define TIMER_TICK_HZ 100
#define TIMER_RELOAD_VAL (SYSTICK_FREQ_HZ / TIMER_TICK_HZ)

/* SysTick reload register is 24-bit */
#define SYSTICK_MAX_RELOAD 0xFFFFFF
#define SYSTICK_MIN_RELOAD                                                     \
  100 /* Minimum reasonable reload value for proper timing */

pok_ret_t pok_timer_init(void) {
  /* Enhanced SysTick reload validation for all clock configurations */

  /* Check for division by zero or invalid tick rate */
  if (TIMER_TICK_HZ == 0) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: TIMER_TICK_HZ cannot be zero\n");
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Check for invalid system frequency */
  if (SYSTICK_FREQ_HZ == 0) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: SYSTICK_FREQ_HZ cannot be zero\n");
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Validate reload value is reasonable (not too small) */
  if (TIMER_RELOAD_VAL < SYSTICK_MIN_RELOAD) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: SysTick reload value %u too small (min %u for stability)\n",
           TIMER_RELOAD_VAL, SYSTICK_MIN_RELOAD);
    printf("System freq: %u Hz, Tick rate: %u Hz\n", SYSTICK_FREQ_HZ,
           TIMER_TICK_HZ);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Validate SysTick reload value doesn't exceed 24-bit limit */
  if (TIMER_RELOAD_VAL > SYSTICK_MAX_RELOAD) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: SysTick reload value %u exceeds 24-bit limit %u\n",
           TIMER_RELOAD_VAL, SYSTICK_MAX_RELOAD);
    printf("Consider reducing SYSTICK_FREQ_HZ or increasing TIMER_TICK_HZ\n");
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Validate that actual tick frequency will be reasonable - use integer
   * arithmetic to avoid FP */
  uint32_t actual_freq = SYSTICK_FREQ_HZ / TIMER_RELOAD_VAL;
  /* Check tolerance using cross-multiplication: actual_freq * 100 vs
   * TIMER_TICK_HZ * [95,105] */
  int64_t actual_freq_scaled = (int64_t)actual_freq * 100;
  int64_t target_freq_lower = (int64_t)TIMER_TICK_HZ * 95; /* 95% lower bound */
  int64_t target_freq_upper =
      (int64_t)TIMER_TICK_HZ * 105; /* 105% upper bound */

  if (actual_freq_scaled < target_freq_lower ||
      actual_freq_scaled > target_freq_upper) {
#ifdef POK_NEEDS_DEBUG
    printf("WARNING: Actual tick frequency %u Hz differs from target %u Hz\n",
           actual_freq, TIMER_TICK_HZ);
    printf("Clock configuration may need adjustment\n");
#endif
  }

#ifdef POK_NEEDS_DEBUG
  printf("SysTick: %u Hz system clock, %u Hz tick rate, reload = %u\n",
         SYSTICK_FREQ_HZ, TIMER_TICK_HZ, TIMER_RELOAD_VAL);
#endif

  /* Disable SysTick */
  SYSTICK_CSR = 0;

  /* Set reload value for desired tick rate */
  SYSTICK_RVR = TIMER_RELOAD_VAL - 1;

  /* Clear current value */
  SYSTICK_CVR = 0;

  /* Configure SysTick: enable, interrupt, use processor clock */
  SYSTICK_CSR =
      SYSTICK_CSR_ENABLE | SYSTICK_CSR_TICKINT | SYSTICK_CSR_CLKSOURCE;

  /* Data Synchronization Barrier to ensure SysTick configuration completes */
  __asm volatile("dsb" : : : "memory");
  __asm volatile("isb" : : : "memory");

  return POK_ERRNO_OK;
}

void pok_timer_handler(void) {
  /* Clear SysTick interrupt flag (automatically cleared by reading CSR) */
  (void)SYSTICK_CSR;

  /* Update POK system time */
  pok_tick_counter++;

  /* Trigger scheduler if needed */
  (void)pok_sched_end_period();
}
