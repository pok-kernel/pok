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

#ifndef __POK_ARM_TYPES_H__
#define __POK_ARM_TYPES_H__

/* Standard integer types for ARM Cortex-M */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* Pointer types for ARM Cortex-M (32-bit architecture) */
typedef unsigned int size_t;
typedef unsigned int uintptr_t;
typedef signed int intptr_t;

#endif /* !__POK_ARM_TYPES_H__ */
