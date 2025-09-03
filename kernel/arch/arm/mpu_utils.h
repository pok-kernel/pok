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

#ifndef __POK_ARM_MPU_UTILS_H__
#define __POK_ARM_MPU_UTILS_H__

#include "cortex_m_config.h"
#include <stdint.h>
#include <types.h>

/**
 * \file    arch/arm/mpu_utils.h
 * \brief   Shared MPU alignment and utility functions
 * \author  POK team
 */

#define MPU_MIN_REGION_SIZE CORTEX_M_MPU_MIN_REGION_SIZE

/**
 * Round up size to next power of 2, with minimum of MPU_MIN_REGION_SIZE
 *
 * @param size Size to round up
 * @return Next power of 2 >= size, minimum MPU_MIN_REGION_SIZE
 */
static inline uint32_t mpu_align_size_to_power_of_2(uint32_t size) {
  if (size <= MPU_MIN_REGION_SIZE) {
    return MPU_MIN_REGION_SIZE;
  }

  /* Find next power of 2 using efficient builtin (O(1) vs O(log n)) */
  if ((size & (size - 1)) == 0) {
    return size; /* Already power of 2 */
  }

  /* Round up to next power of 2 using count leading zeros */
  return 1U << (32 - __builtin_clz(size - 1));
}

/**
 * Check if a value is power of 2
 *
 * @param value Value to check
 * @return true if power of 2, false otherwise
 */
static inline pok_bool_t mpu_is_power_of_2(uint32_t value) {
  return (value != 0) && ((value & (value - 1)) == 0);
}

/**
 * Check if address is aligned to size
 *
 * @param addr Address to check
 * @param size Alignment size (must be power of 2)
 * @return TRUE if aligned, FALSE otherwise
 */
static inline pok_bool_t mpu_is_aligned(uint32_t addr, uint32_t size) {
  return (addr & (size - 1)) == 0;
}

#endif /* !__POK_ARM_MPU_UTILS_H__ */
