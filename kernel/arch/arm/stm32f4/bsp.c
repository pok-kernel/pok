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
 * \file    arch/arm/stm32f4/bsp.c
 * \author  POK team
 * \brief   STM32F4 Board Support Package
 */

#include "clock_config.h"
#include "peripherals.h"
#include <bsp.h>
#include <errno.h>
#include <libc.h>

/* STM32F4-specific memory configuration overrides */
#define POK_FLASH_BASE STM32F4_FLASH_BASE
#define POK_SRAM_BASE STM32F4_SRAM_BASE

/* STM32F4 SRAM size - configurable for different MCU variants
 * STM32F407VG/417VG: 128KB main SRAM + 64KB CCM
 * STM32F429/439: 256KB main SRAM + 64KB CCM
 * Override POK_SRAM_SIZE in platform-specific headers if needed */
#ifndef POK_SRAM_SIZE
#define POK_SRAM_SIZE 0x20000 /* Default: 128KB for STM32F407/417 */
#endif

#define POK_KERNEL_MEMORY_SIZE 0x8000 /* 32KB for kernel */

/* Include configurable memory layout */
#include "../memory_config.h"

/* Forward declarations for STM32F4 specific functions */
pok_ret_t pok_cons_init(void);
pok_ret_t pok_timer_init(void);
pok_ret_t pok_stm32f4_clock_init(void);

/* Simple kernel memory allocator - allocates from kernel space for stacks, etc.
 */
static uint32_t current_alloc_addr = POK_KERNEL_MEMORY_BASE;

/**
 * Initialize STM32F4 Board Support Package
 *
 * Sets up system clocks, console UART, and system timer.
 * Must be called early in system initialization.
 *
 * @return POK_ERRNO_OK on success, error code on failure
 */
pok_ret_t pok_bsp_init(void) {
  pok_ret_t ret;

  /* Initialize system clocks */
  ret = pok_stm32f4_clock_init();
  if (ret != POK_ERRNO_OK) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Clock initialization failed: %d\n", ret);
#endif
    return (ret);
  }

  /* Initialize console */
  ret = pok_cons_init();
  if (ret != POK_ERRNO_OK) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Console initialization failed: %d\n", ret);
#endif
    return (ret);
  }

  /* Initialize timer */
  ret = pok_timer_init();
  if (ret != POK_ERRNO_OK) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Timer initialization failed: %d\n", ret);
#endif
    return (ret);
  }

  return POK_ERRNO_OK;
}

void *pok_bsp_mem_alloc(size_t size) {
  void *ret;

  if (size == 0) {
    return (NULL);
  }

  /* Align to 8-byte boundary */
  size = (size + POK_MEMORY_ALIGNMENT_MASK) & ~POK_MEMORY_ALIGNMENT_MASK;

  /* Check if we have enough kernel memory remaining - use subtraction to
   * prevent overflow */
  uint32_t kernel_end;

  /* Prevent overflow in kernel_end calculation */
  if (POK_KERNEL_MEMORY_SIZE > (0xFFFFFFFFU - POK_KERNEL_MEMORY_BASE)) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Kernel memory configuration would overflow address space\n");
#endif
    return (NULL);
  }

  kernel_end = POK_KERNEL_MEMORY_BASE + POK_KERNEL_MEMORY_SIZE;

  /* Additional sanity check: ensure current_alloc_addr is within valid kernel
   * range */
  if (current_alloc_addr < POK_KERNEL_MEMORY_BASE ||
      current_alloc_addr > kernel_end) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Kernel allocator corrupted. current_alloc_addr=0x%x\n",
           current_alloc_addr);
#endif
    return (NULL);
  }

  if (size > (kernel_end - current_alloc_addr)) {
#ifdef POK_NEEDS_DEBUG
    printf(
        "ERROR: Kernel memory exhausted. Requested: %u, Available: %lu\n", size,
        (POK_KERNEL_MEMORY_BASE + POK_KERNEL_MEMORY_SIZE) - current_alloc_addr);
#endif
    return (NULL);
  }

  ret = (void *)current_alloc_addr;
  current_alloc_addr += size;

#ifdef POK_NEEDS_DEBUG
  printf("Allocated %u bytes at 0x%x (kernel space)\n", size, (uint32_t)ret);
#endif

  return (ret);
}

uintptr_t pok_bsp_mem_base(void) { return (POK_USER_MEMORY_BASE); }

size_t pok_bsp_mem_size(void) { return (POK_USER_MEMORY_SIZE); }

uintptr_t pok_bsp_kernel_base(void) { return (POK_KERNEL_MEMORY_BASE); }

size_t pok_bsp_kernel_size(void) { return (POK_KERNEL_REGION_SIZE); }

/**
 * Initialize STM32F4 system clocks
 * Configures PLL for 168MHz operation using 8MHz HSE crystal
 *
 * @return POK_ERRNO_OK on success, error code on failure
 */
pok_ret_t pok_stm32f4_clock_init(void) {
  /* Validate HSE frequency - PLL calculations are hardcoded for 8MHz */
  if (HSE_FREQ_HZ != 8000000) {
#ifdef POK_NEEDS_DEBUG
    printf(
        "WARNING: HSE frequency is %d Hz, but PLL configuration assumes 8MHz\n",
        HSE_FREQ_HZ);
    printf(
        "         Clock frequencies may be incorrect. Update PLL_M divisor.\n");
#endif
  }

  /* Validate USB clock calculation for current HSE */
  /* USB_CLK = (HSE * PLL_N / PLL_M) / PLL_Q = (HSE * 336 / 8) / 7 */
  uint32_t calculated_usb_freq = ((uint32_t)HSE_FREQ_HZ * 336U / 8U) / 7U;
  if (calculated_usb_freq != USB_FREQ_HZ) {
#ifdef POK_NEEDS_DEBUG
    printf("WARNING: USB clock will be %d Hz, but USB requires exactly 48MHz\n",
           calculated_usb_freq);
    printf("         Adjust PLL_Q divisor if USB functionality is needed.\n");
#endif
  }

  volatile uint32_t *RCC_CR =
      (volatile uint32_t *)(RCC_BASE + 0x00); /* RCC Clock Control Register */
  volatile uint32_t *RCC_PLLCFGR =
      (volatile uint32_t *)(RCC_BASE +
                            0x04); /* RCC PLL Configuration Register */
  volatile uint32_t *RCC_CFGR =
      (volatile uint32_t *)(RCC_BASE +
                            0x08); /* RCC Clock Configuration Register */
  volatile uint32_t *FLASH_ACR =
      (volatile uint32_t *)(STM32F4_FLASH_CTRL_BASE +
                            0x00); /* Flash Access Control Register */

  uint32_t timeout = 0;

  /* Set flash latency for 168MHz operation (5 wait states for 150-168MHz
   * at 3.3V) */
  *FLASH_ACR = (*FLASH_ACR & ~0x7) | FLASH_LATENCY;

  /* Enable HSE (High Speed External clock) */
  *RCC_CR |= (1 << 16); /* HSEON = 1 */

  /* Wait for HSE to be ready */
  timeout = 10000;
  while (!((*RCC_CR) & (1 << 17)) && timeout > 0) { /* Wait for HSERDY = 1 */
    timeout--;
  }

  if (timeout == 0) {
    return POK_ERRNO_EFAULT; /* HSE failed to start */
  }

  /* Configure voltage regulator scaling for 168MHz operation
   * VOS = Scale 1 mode (required for frequencies > 144 MHz) */
  volatile uint32_t *PWR_CR = (uint32_t *)STM32F4_PWR_BASE;

  /* Enable PWR clock in RCC */
  volatile uint32_t *RCC_APB1ENR = (uint32_t *)(STM32F4_RCC_BASE + 0x40);
  *RCC_APB1ENR |= (1 << 28); /* PWREN = 1 */

  /* Set VOS to Scale 1 (highest performance, required for 168MHz) */
  *PWR_CR =
      (*PWR_CR & ~(3 << 14)) | (3 << 14); /* VOS[1:0] = 11 (Scale 1 mode) */

  /* Wait for voltage regulator to be ready */
  timeout = 1000;
  volatile uint32_t *PWR_CSR = (uint32_t *)(STM32F4_PWR_BASE + 0x04);
  while (!((*PWR_CSR) & (1 << 14)) && timeout > 0) { /* Wait for VOSRDY = 1 */
    timeout--;
  }

  /* Configure PLL:
   * PLL_M = 8 (HSE/8 = 1MHz)
   * PLL_N = 336 (1MHz * 336 = 336MHz)
   * PLL_P = 2 (336MHz / 2 = 168MHz SYSCLK)
   * PLL_Q = 7 (336MHz / 7 = 48MHz for USB)
   * HSE as PLL source
   */
  *RCC_PLLCFGR = (7 << 24) |  /* PLL_Q = 7 */
                 (0 << 16) |  /* PLL_P = 0 (means /2) */
                 (336 << 6) | /* PLL_N = 336 */
                 (8 << 0) |   /* PLL_M = 8 */
                 (1 << 22);   /* HSE as PLL source */

  /* Enable PLL */
  *RCC_CR |= (1 << 24); /* PLLON = 1 */

  /* Wait for PLL to be ready */
  timeout = 10000;
  while (!((*RCC_CR) & (1 << 25)) && timeout > 0) { /* Wait for PLLRDY = 1 */
    timeout--;
  }

  if (timeout == 0) {
    return POK_ERRNO_EFAULT; /* PLL failed to lock */
  }

  /* Configure bus prescalers:
   * AHB Prescaler: /1 (168MHz)
   * APB1 Prescaler: /4 (42MHz, max 42MHz)
   * APB2 Prescaler: /2 (84MHz, max 84MHz)
   */
  *RCC_CFGR = (0x0 << 4) |  /* AHB prescaler /1 */
              (0x5 << 10) | /* APB1 prescaler /4 */
              (0x4 << 13);  /* APB2 prescaler /2 */

  /* Switch system clock to PLL */
  *RCC_CFGR = (*RCC_CFGR & ~0x3) | 0x2; /* SW = 10 (PLL as system clock) */

  /* Wait for PLL to be used as system clock */
  timeout = 10000;
  while (((*RCC_CFGR & 0xC) >> 2) != 0x2 &&
         timeout > 0) { /* Wait for SWS = 10 */
    timeout--;
  }

  if (timeout == 0) {
    return POK_ERRNO_EFAULT; /* Failed to switch to PLL */
  }

#ifdef POK_NEEDS_DEBUG
  printf("STM32F4 clock configured: SYSCLK=%dMHz, AHB=%dMHz, APB1=%dMHz, "
         "APB2=%dMHz\n",
         SYSCLK_FREQ_HZ / 1000000, AHB_FREQ_HZ / 1000000,
         APB1_FREQ_HZ / 1000000, APB2_FREQ_HZ / 1000000);
#endif

  return POK_ERRNO_OK;
}
