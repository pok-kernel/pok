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
 * \file    arch/arm/stm32f4/cons.c
 * \author  POK team
 * \brief   STM32F4 console implementation via USART
 */

#include "clock_config.h"
#include "peripherals.h"
#include <errno.h>
#include <libc.h>
#include <types.h> /* For pok_ret_t, pok_bool_t, uint32_t, uint64_t */

/* USART configuration constants */
#define UART_BAUD_RATE 115200
#define USART_DEFAULT_BAUD_RATE 115200
#define USART_OVERSAMPLING_FACTOR 16
#define USART_ROUNDING_DIVISOR 8 /* For rounding in baud rate calculation */

/* STM32F4 USART1 registers */
/* USART1_BASE now defined in peripherals.h */
#define USART1_SR (*((volatile uint32_t *)(USART1_BASE + 0x00)))
#define USART1_DR (*((volatile uint32_t *)(USART1_BASE + 0x04)))
#define USART1_BRR (*((volatile uint32_t *)(USART1_BASE + 0x08)))
#define USART1_CR1 (*((volatile uint32_t *)(USART1_BASE + 0x0C)))
#define USART1_CR2 (*((volatile uint32_t *)(USART1_BASE + 0x10)))
#define USART1_CR3 (*((volatile uint32_t *)(USART1_BASE + 0x14)))

/* USART status register bits */
#define USART_SR_TXE (1 << 7)  /* Transmit data register empty */
#define USART_SR_TC (1 << 6)   /* Transmission complete */
#define USART_SR_RXNE (1 << 5) /* Read data register not empty */
#define USART_SR_ORE (1 << 3)  /* Overrun error */
#define USART_SR_FE (1 << 1)   /* Framing error */
#define USART_SR_PE (1 << 0)   /* Parity error */

/* USART control register 1 bits */
#define USART_CR1_UE (1 << 13) /* USART enable */
#define USART_CR1_TE (1 << 3)  /* Transmitter enable */
#define USART_CR1_RE (1 << 2)  /* Receiver enable */

/* RCC registers for clock control */
/* RCC_BASE now defined in peripherals.h */
#define RCC_APB2ENR (*((volatile uint32_t *)(RCC_BASE + 0x44)))
#define RCC_AHB1ENR (*((volatile uint32_t *)(RCC_BASE + 0x30)))

#define RCC_APB2ENR_USART1EN (1 << 4)
#define RCC_AHB1ENR_GPIOAEN (1 << 0)

/* GPIO registers for USART pins */
/* GPIOA_BASE now defined in peripherals.h */
#define GPIOA_MODER                                                            \
  (*((volatile uint32_t *)(GPIOA_BASE + 0x00))) /* Mode register */
#define GPIOA_OTYPER                                                           \
  (*((volatile uint32_t *)(GPIOA_BASE + 0x04))) /* Output type register */
#define GPIOA_OSPEEDR                                                          \
  (*((volatile uint32_t *)(GPIOA_BASE + 0x08))) /* Output speed register */
#define GPIOA_PUPDR                                                            \
  (*((volatile uint32_t *)(GPIOA_BASE + 0x0C))) /* Pull-up/pull-down register  \
                                                 */
#define GPIOA_AFRL (*((volatile uint32_t *)(GPIOA_BASE + 0x20))) /* AF[7:0] */
#define GPIOA_AFRH                                                             \
  (*((volatile uint32_t *)(GPIOA_BASE + 0x24))) /* AF[15:8]                    \
                                                 */

/* GPIO pin definitions for PA9 (TX) and PA10 (RX) */
#define PA9_PIN_POS 9
#define PA10_PIN_POS 10
#define PA9_MODER_POS                                                          \
  (PA9_PIN_POS * 2) /* 18 - Mode register uses 2 bits per pin */
#define PA10_MODER_POS                                                         \
  (PA10_PIN_POS * 2) /* 20 - Mode register uses 2 bits per pin */
/* Define OSPEEDR bit positions (2 bits per pin) */
#define PA9_OSPEEDR_POS (PA9_PIN_POS * 2)
#define PA10_OSPEEDR_POS (PA10_PIN_POS * 2)
#define PA9_AF_POS 4  /* AFRH register: PA9 uses bits 4-7 */
#define PA10_AF_POS 8 /* AFRH register: PA10 uses bits 8-11 */

/* GPIO mode values */
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_AF 2 /* Alternate function */
#define GPIO_MODE_ANALOG 3

/* GPIO speed values */
#define GPIO_SPEED_LOW 0       /* 2 MHz */
#define GPIO_SPEED_MEDIUM 1    /* 25 MHz */
#define GPIO_SPEED_HIGH 2      /* 50 MHz */
#define GPIO_SPEED_VERY_HIGH 3 /* 100 MHz */

/* GPIO pull-up/pull-down values */
#define GPIO_PUPD_NONE 0
#define GPIO_PUPD_UP 1
#define GPIO_PUPD_DOWN 2

/* Alternate function values */
#define GPIO_AF7_USART 7

pok_ret_t pok_cons_init(void) {
  /* Enable GPIOA and USART1 clocks */
  RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

  /* Configure PA9 (TX) and PA10 (RX) as alternate function */
  GPIOA_MODER &= ~((3 << PA9_MODER_POS) |
                   (3 << PA10_MODER_POS)); /* Clear mode bits for PA9, PA10 */
  GPIOA_MODER |=
      (GPIO_MODE_AF << PA9_MODER_POS) |
      (GPIO_MODE_AF << PA10_MODER_POS); /* Set alternate function mode */

  /* Set alternate function 7 (USART) for PA9 and PA10 */
  /* PA9 = pin 9 (AFRH bit 4-7), PA10 = pin 10 (AFRH bit 8-11) */
  GPIOA_AFRH &= ~((0xF << PA9_AF_POS) |
                  (0xF << PA10_AF_POS)); /* Clear AF bits in AFRH register */
  GPIOA_AFRH |= (GPIO_AF7_USART << PA9_AF_POS) |
                (GPIO_AF7_USART << PA10_AF_POS); /* Set AF7 for PA9 and PA10 */

  /* Configure output type as push-pull (default, but explicit) */
  GPIOA_OTYPER &= ~((1 << PA9_PIN_POS) |
                    (1 << PA10_PIN_POS)); /* PA9, PA10 push-pull output */

  /* Configure high speed for 115200 baud reliability */
  GPIOA_OSPEEDR &= ~((3u << PA9_OSPEEDR_POS) |
                     (3u << PA10_OSPEEDR_POS)); /* Clear speed bits */
  GPIOA_OSPEEDR |= ((uint32_t)GPIO_SPEED_VERY_HIGH << PA9_OSPEEDR_POS) |
                   ((uint32_t)GPIO_SPEED_VERY_HIGH
                    << PA10_OSPEEDR_POS); /* Set very high speed */

  /* Configure pull-up for RX, no pull for TX (USART idle high; TX is driven) */
  GPIOA_PUPDR &=
      ~((3 << PA9_MODER_POS) | (3 << PA10_MODER_POS)); /* Clear pull bits */
  GPIOA_PUPDR |= (GPIO_PUPD_UP
                  << PA10_MODER_POS); /* PA10 (RX) pull-up, PA9 (TX) no pull */
  /* Configure USART1 baud rate */
  /* For oversampling by 16: BRR = (mantissa << 4) + fraction */
  /* USARTDIV = f_CK / (16 * baud_rate) */
  uint32_t apb2_clock = APB2_FREQ_HZ; /* Use correct 84MHz APB2 clock */
  uint32_t baud_rate = USART_DEFAULT_BAUD_RATE;

  /* Bounds checking for baud rate calculation */
  if (baud_rate == 0) {
    return POK_ERRNO_EINVAL; /* Avoid division by zero */
  }

  /* Check for potential overflow in calculation */
  if (apb2_clock > (0xFFFFFFFFU / USART_OVERSAMPLING_FACTOR)) {
    return POK_ERRNO_EINVAL; /* Clock frequency too high for safe calculation */
  }

  /* Use 64-bit arithmetic to prevent overflow during computation */
  uint64_t numerator = ((uint64_t)apb2_clock * USART_OVERSAMPLING_FACTOR) +
                       (USART_ROUNDING_DIVISOR * baud_rate);
  uint64_t denominator = USART_OVERSAMPLING_FACTOR * baud_rate;
  uint32_t usartdiv_scaled = (uint32_t)(numerator / denominator);

  /* Extract mantissa (integer part) and fraction (4-bit fractional part) */
  uint32_t mantissa = usartdiv_scaled / USART_OVERSAMPLING_FACTOR;
  uint32_t fraction = usartdiv_scaled % USART_OVERSAMPLING_FACTOR;

  /* Validate that mantissa fits in 12 bits (STM32F4 BRR register limit) */
  if (mantissa > 0xFFF) {
    return POK_ERRNO_EINVAL; /* Baud rate too low for this clock frequency */
  }

  /* Pack into BRR register format: mantissa[15:4] | fraction[3:0] */
  USART1_BRR = (mantissa << 4) | (fraction & 0x0F);

  /* Enable USART, transmitter, and receiver */
  USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

  return POK_ERRNO_OK;
}

pok_bool_t pok_cons_write(const char *s, size_t length) {
  if (s == NULL) {
    return FALSE;
  }

  for (size_t i = 0; i < length; i++) {
    /* Wait for transmit data register to be empty with timeout protection */
    /* Scale timeout based on APB2 clock (USART1 is on APB2) and baud rate */
    uint32_t timeout = (APB2_FREQ_HZ / UART_BAUD_RATE) * 10; /* ~10 bit times */
    while (!(USART1_SR & USART_SR_TXE) && timeout > 0) {
      timeout--;
    }
    if (timeout == 0) {
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: UART transmit timeout on character %zu\n", i);
#endif
      return FALSE;
    }

    /* Send character */
    USART1_DR = (uint8_t)s[i];
  }

  /* Wait for transmission complete with timeout protection */
  uint32_t timeout = (APB2_FREQ_HZ / UART_BAUD_RATE) * 10; /* ~10 bit times */
  while (!(USART1_SR & USART_SR_TC) && timeout > 0) {
    timeout--;
  }
  if (timeout == 0) {
#ifdef POK_NEEDS_DEBUG
    printf("WARNING: UART transmission complete timeout\n");
#endif
    /* Don't return error for final flush timeout, data likely sent */
  }

  return TRUE;
}

pok_ret_t pok_cons_read(char *s, size_t length) {
  if (s == NULL) {
    return POK_ERRNO_EINVAL;
  }

  for (size_t i = 0; i < length; i++) {
    /* Wait for receive data register to have data with timeout protection */
    uint32_t timeout =
        (APB2_FREQ_HZ / UART_BAUD_RATE) * 20; /* ~20 bit times for RX */
    while (!(USART1_SR & USART_SR_RXNE) && timeout > 0) {
      timeout--;
    }
    if (timeout == 0) {
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: UART receive timeout on character %zu\n", i);
#endif
      return POK_ERRNO_EFAULT;
    }

    /* Check and clear UART error conditions (ORE/FE/PE) */
    uint32_t status = USART1_SR;
    if (status & (USART_SR_ORE | USART_SR_FE | USART_SR_PE)) {
      /* Clear errors by reading SR then DR */
      (void)USART1_SR; /* Reading SR */
      (void)USART1_DR; /* Reading DR clears error flags */
#ifdef POK_NEEDS_DEBUG
      if (status & USART_SR_ORE)
        printf("UART Overrun Error cleared\n");
      if (status & USART_SR_FE)
        printf("UART Framing Error cleared\n");
      if (status & USART_SR_PE)
        printf("UART Parity Error cleared\n");
#endif
      return POK_ERRNO_EFAULT; /* I/O error */
    }

    /* Read character */
    s[i] = USART1_DR & 0xFF;
  }

  return POK_ERRNO_OK;
}

/**
 * Get a single character from console (blocking)
 *
 * @param c Pointer to store the received character
 */
void pok_cons_get_char(char *c) {
  if (c == NULL) {
    return;
  }

  /* Wait for receive data register to have data */
  while (!(USART1_SR & USART_SR_RXNE)) {
    /* Busy wait - no timeout for single char read */
  }

  /* Read character */
  *c = (char)(USART1_DR & 0xFF);
}
