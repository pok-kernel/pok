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

#ifndef __POK_STM32F4_CLOCK_CONFIG_H__
#define __POK_STM32F4_CLOCK_CONFIG_H__

/**
 * \file    clock_config.h
 * \brief   STM32F4 clock configuration constants
 * \author  POK team
 *
 * STM32F407VG typical clock configuration with 8MHz HSE crystal:
 * - HSE: 8MHz (external crystal)
 * - PLL_M: 8 (HSE/8 = 1MHz)
 * - PLL_N: 336 (1MHz * 336 = 336MHz)
 * - PLL_P: 2 (336MHz / 2 = 168MHz SYSCLK)
 * - PLL_Q: 7 (336MHz / 7 = 48MHz for USB)
 * - AHB Prescaler: /1 (168MHz)
 * - APB1 Prescaler: /4 (42MHz, max 42MHz)
 * - APB2 Prescaler: /2 (84MHz, max 84MHz)
 */

/* High-Speed External oscillator frequency */
#ifndef HSE_FREQ_HZ
#define HSE_FREQ_HZ 8000000 /* 8MHz crystal on STM32F4DISCOVERY */
#endif

/* System clock frequencies after PLL configuration */
#define SYSCLK_FREQ_HZ 168000000 /* Main system clock */

/* Bus prescaler definitions */
#define AHB_PRESCALER 1  /* AHB Prescaler from SYSCLK */
#define APB1_PRESCALER 4 /* APB1 Prescaler from SYSCLK */
#define APB2_PRESCALER 2 /* APB2 Prescaler from SYSCLK */

/* Bus clock frequencies - derived from prescalers */
#define AHB_FREQ_HZ                                                            \
  (SYSCLK_FREQ_HZ / AHB_PRESCALER) /* AHB bus clock (HCLK)                     \
                                    */
#define APB1_FREQ_HZ                                                           \
  (AHB_FREQ_HZ / APB1_PRESCALER) /* APB1 bus clock (PCLK1) - max 42MHz */
#define APB2_FREQ_HZ                                                           \
  (AHB_FREQ_HZ / APB2_PRESCALER) /* APB2 bus clock (PCLK2) - max 84MHz */

/* Timer clock frequencies - correct calculation based on actual prescaler
 * values STM32 rule: Timer clock = APBx clock * 2 when APB prescaler > 1,
 * otherwise APBx clock * 1 Current config: APB1 prescaler = /4 (>1), APB2
 * prescaler = /2 (>1)
 */

/* Calculate timer frequencies based on actual prescaler values */
#define APB1_TIMER_FREQ_HZ                                                     \
  ((APB1_PRESCALER > 1) ? (APB1_FREQ_HZ * 2)                                   \
                        : APB1_FREQ_HZ) /* 84MHz when prescaler=4 */
#define APB2_TIMER_FREQ_HZ                                                     \
  ((APB2_PRESCALER > 1) ? (APB2_FREQ_HZ * 2)                                   \
                        : APB2_FREQ_HZ) /* 168MHz when prescaler=2 */

/* SysTick uses SYSCLK by default */
#define SYSTICK_FREQ_HZ SYSCLK_FREQ_HZ

/* USB clock frequency (must be 48MHz) */
#define USB_FREQ_HZ 48000000

/* Flash latency for 168MHz operation at 3.3V */
#define FLASH_LATENCY 5 /* 5 wait states for 150-168MHz */

#endif /* !__POK_STM32F4_CLOCK_CONFIG_H__ */
