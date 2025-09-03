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

#ifndef __POK_STM32F4_PERIPHERALS_H__
#define __POK_STM32F4_PERIPHERALS_H__

/**
 * \file    arch/arm/stm32f4/peripherals.h
 * \brief   STM32F4 peripheral base addresses and register definitions
 * \author  POK team
 */

/* STM32F4 Memory Map */
#define STM32F4_FLASH_BASE 0x08000000UL
#define STM32F4_FLASH_SIZE 0x100000UL /* 1MB flash (STM32F407/417) */
#define STM32F4_SRAM_BASE 0x20000000UL
#define STM32F4_CCMSRAM_BASE 0x10000000UL
#define STM32F4_CCMSRAM_SIZE 0x10000UL /* 64KB CCM SRAM (STM32F407/417) */

/* APB1 Peripherals */
#define STM32F4_APB1_BASE 0x40000000UL
#define STM32F4_PWR_BASE 0x40007000UL /* Power control for VOS scaling */

/* APB2 Peripherals */
#define STM32F4_APB2_BASE 0x40010000UL
#define STM32F4_USART1_BASE 0x40011000UL

/* AHB1 Peripherals */
#define STM32F4_AHB1_BASE 0x40020000UL
#define STM32F4_GPIOA_BASE 0x40020000UL
#define STM32F4_RCC_BASE 0x40023800UL

/* System Control Space */
#define STM32F4_SCS_BASE 0xE000E000UL
#define STM32F4_SYSTICK_BASE 0xE000E010UL
#define STM32F4_NVIC_BASE 0xE000E100UL
#define STM32F4_SCB_BASE 0xE000ED00UL
#define STM32F4_MPU_BASE 0xE000ED90UL

/* Flash Control */
#define STM32F4_FLASH_CTRL_BASE 0x40023C00UL

/* Convenience macros */
#define USART1_BASE STM32F4_USART1_BASE
#define GPIOA_BASE STM32F4_GPIOA_BASE
#define RCC_BASE STM32F4_RCC_BASE
#define PWR_BASE STM32F4_PWR_BASE /* Power control for VOS scaling */
#define SYSTICK_BASE STM32F4_SYSTICK_BASE

#endif /* !__POK_STM32F4_PERIPHERALS_H__ */
