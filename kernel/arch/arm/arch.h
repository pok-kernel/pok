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
 * \file    arch/arm/arch.h
 * \author  POK team
 * \brief   ARM Cortex-M architecture constants and definitions
 */

#ifndef __POK_ARM_ARCH_H__
#define __POK_ARM_ARCH_H__

/* Required includes for type definitions */
#include <types.h>

/* ARM Cortex-M Exception Return Values */
#define ARM_EXC_RETURN_THREAD_PSP                                              \
  0xFFFFFFFDUL /* Return to Thread mode, use PSP */
#define ARM_EXC_RETURN_HANDLER_MSP                                             \
  0xFFFFFFF1UL /* Return to Handler mode, use MSP */
#define ARM_EXC_RETURN_THREAD_MSP                                              \
  0xFFFFFFF9UL /* Return to Thread mode, use MSP */

/* ARM Cortex-M Register Bit Masks */
#define ARM_REGISTER_BYTE_MASK 0xFFu        /* 8-bit register mask */
#define ARM_REGISTER_WORD_MASK 0xFFFFu      /* 16-bit register mask */
#define ARM_REGISTER_DWORD_MASK 0xFFFFFFFFu /* 32-bit register mask */

/* Fault Status Register Masks */
#define ARM_CFSR_MMFSR_MASK 0xFFu      /* MemManage Fault Status [7:0] */
#define ARM_CFSR_BFSR_MASK 0xFF00u     /* Bus Fault Status [15:8] */
#define ARM_CFSR_UFSR_MASK 0xFFFF0000u /* Usage Fault Status [31:16] */

/* MPU Region Size Constants */
#define ARM_MPU_MIN_SUBREGION_SIZE 256 /* Minimum size for subregions */
#define ARM_MPU_SUBREGION_COUNT 8      /* Number of subregions per region */

/* SVC Instruction Encoding */
#define ARM_SVC_NUMBER_MASK 0xFF /* SVC number in lower 8 bits */

/* Priority Register Masks with range validation */
#ifndef ARM_PRIORITY_BITS
#ifdef __NVIC_PRIO_BITS
#define ARM_PRIORITY_BITS __NVIC_PRIO_BITS
#else
#define ARM_PRIORITY_BITS 4 /* BSP default; override per SoC */
#endif
#endif

/* Validate ARM_PRIORITY_BITS is within valid range */
#if ARM_PRIORITY_BITS < 2 || ARM_PRIORITY_BITS > 8
#error "ARM_PRIORITY_BITS must be between 2 and 8 inclusive"
#endif

/* Calculate priority mask based on validated priority bits */
#define ARM_PRIORITY_MASK ((uint8_t)(0xFFu << (8 - ARM_PRIORITY_BITS)))

/* Priority level constants for validated range */
#define ARM_PRIORITY_LEVELS (1u << ARM_PRIORITY_BITS)
#define ARM_MAX_PRIORITY_VALUE (ARM_PRIORITY_LEVELS - 1)

/* Common Magic Values - for reference only, use descriptive names */
#define ARM_MAGIC_DEAD 0xDEADu /* Debug marker value */
#define ARM_MAGIC_BEEF 0xBEEFu /* Debug marker value */

#endif /* __POK_ARM_ARCH_H__ */
