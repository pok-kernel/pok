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

#ifndef __POK_ARM_CORTEX_M_CONFIG_H__
#define __POK_ARM_CORTEX_M_CONFIG_H__

#include <types.h>

/**
 * \file    arch/arm/cortex_m_config.h
 * \brief   ARM Cortex-M configuration constants and limits
 * \author  POK team
 *
 * This file centralizes hardware-specific constants for ARM Cortex-M
 * microcontrollers to make it easier to support different variants.
 */

/* NVIC (Nested Vectored Interrupt Controller) configuration */
#ifndef CORTEX_M_NVIC_VECTOR_COUNT
#define CORTEX_M_NVIC_VECTOR_COUNT                                             \
  98 /* 16 system + 82 external interrupts (default: STM32F4-compatible) */
#endif

#define CORTEX_M_NVIC_VECTOR_TABLE_SIZE                                        \
  (CORTEX_M_NVIC_VECTOR_COUNT * 4) /* Each vector is 4 bytes */

/* Calculate next power-of-two for vector table alignment */
#ifndef CORTEX_M_NEXT_POW2
#define CORTEX_M_NEXT_POW2(x) (1U << (32 - __builtin_clz((uint32_t)((x) - 1))))
#endif

#ifndef CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT
#define CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT                                   \
  CORTEX_M_NEXT_POW2(CORTEX_M_NVIC_VECTOR_TABLE_SIZE)
#endif

/* Compile-time check: vector table alignment must be >= vector table size */
_Static_assert(CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT >=
                   CORTEX_M_NVIC_VECTOR_TABLE_SIZE,
               "Vector table alignment must be at least as large as the vector "
               "table size");
/* Compile-time check: vector table alignment must also be a power of two */
_Static_assert((CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT &
                (CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT - 1)) == 0,
               "Vector table alignment must be a power of two");

/* MPU (Memory Protection Unit) configuration */
#ifndef CORTEX_M_MPU_MAX_REGIONS
#define CORTEX_M_MPU_MAX_REGIONS                                               \
  8 /* Standard Cortex-M3/M4 has 8 MPU regions                                 \
     */
#endif

#ifndef CORTEX_M_MPU_MIN_REGION_SIZE
#define CORTEX_M_MPU_MIN_REGION_SIZE 32 /* Minimum MPU region size in bytes */
#endif

/* Thread/Stack configuration */
#ifndef CORTEX_M_STACK_ALIGNMENT
#define CORTEX_M_STACK_ALIGNMENT                                               \
  8 /* ARM Cortex-M requires 8-byte stack alignment */
#endif

#define CORTEX_M_STACK_ALIGNMENT_MASK (CORTEX_M_STACK_ALIGNMENT - 1)

/* Exception priorities (0 = highest, 255 = lowest for Cortex-M3/M4) */
#ifndef CORTEX_M_PRIORITY_HIGHEST
#define CORTEX_M_PRIORITY_HIGHEST 0
#endif

#ifndef CORTEX_M_PRIORITY_HIGH
#define CORTEX_M_PRIORITY_HIGH 64
#endif

#ifndef CORTEX_M_PRIORITY_NORMAL
#define CORTEX_M_PRIORITY_NORMAL 128
#endif

#ifndef CORTEX_M_PRIORITY_LOW
#define CORTEX_M_PRIORITY_LOW 192
#endif

#ifndef CORTEX_M_PRIORITY_LOWEST
#define CORTEX_M_PRIORITY_LOWEST 255
#endif

#endif /* !__POK_ARM_CORTEX_M_CONFIG_H__ */
