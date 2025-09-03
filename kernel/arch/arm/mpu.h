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

#ifndef __POK_ARM_MPU_H__
#define __POK_ARM_MPU_H__

#include "cortex_m_config.h"
#include <errno.h>
#include <types.h>

/* ARM Cortex-M MPU Register Base */
#define MPU_BASE 0xE000ED90

/* MPU Registers */
#define MPU_TYPE (*((volatile uint32_t *)(MPU_BASE + 0x00)))
#define MPU_CTRL (*((volatile uint32_t *)(MPU_BASE + 0x04)))
#define MPU_RNR (*((volatile uint32_t *)(MPU_BASE + 0x08)))
#define MPU_RBAR (*((volatile uint32_t *)(MPU_BASE + 0x0C)))
#define MPU_RASR (*((volatile uint32_t *)(MPU_BASE + 0x10)))

/* MPU Control Register bits */
#define MPU_CTRL_ENABLE (1 << 0)
#define MPU_CTRL_HFNMIENA (1 << 1)
#define MPU_CTRL_PRIVDEFENA (1 << 2)

/* MPU Region Base Address Register bits */
#define MPU_RBAR_VALID (1 << 4)
#define MPU_RBAR_REGION_MASK 0x0F
#define MPU_RBAR_ADDR_MASK                                                     \
  0xFFFFFFE0 /* Mask for base address (clears region + valid bits) */

/* MPU Region Attribute and Size Register bits */
#define MPU_RASR_ENABLE (1 << 0)
#define MPU_RASR_SIZE_SHIFT 1
#define MPU_RASR_SIZE_MASK (0x1F << MPU_RASR_SIZE_SHIFT)
#define MPU_RASR_SRD_SHIFT 8
#define MPU_RASR_SRD_MASK (0xFF << MPU_RASR_SRD_SHIFT)
#define MPU_RASR_B (1 << 16)
#define MPU_RASR_C (1 << 17)
#define MPU_RASR_S (1 << 18)
#define MPU_RASR_TEX_SHIFT 19
#define MPU_RASR_TEX_MASK (0x7 << MPU_RASR_TEX_SHIFT)
#define MPU_RASR_AP_SHIFT 24
#define MPU_RASR_AP_MASK (0x7 << MPU_RASR_AP_SHIFT)
#define MPU_RASR_XN (1 << 28)

/* Access Permission encoding (pre-shifted for RASR register) */
#define MPU_AP_NO_ACCESS (0x0 << MPU_RASR_AP_SHIFT)
#define MPU_AP_PRIV_RW (0x1 << MPU_RASR_AP_SHIFT)
#define MPU_AP_PRIV_RW_USER_RO (0x2 << MPU_RASR_AP_SHIFT)
#define MPU_AP_ALL_RW (0x3 << MPU_RASR_AP_SHIFT)
#define MPU_AP_PRIV_RO (0x5 << MPU_RASR_AP_SHIFT)
#define MPU_AP_ALL_RO (0x6 << MPU_RASR_AP_SHIFT)

/* Memory attributes (TEX, C, B combinations) */
#define MPU_ATTR_NORMAL (MPU_RASR_C | MPU_RASR_B)
#define MPU_ATTR_DEVICE 0
#define MPU_ATTR_STRONGLY_ORDERED 0

/* TEX/C/B Helper Macros for Common Memory Types */
#define MPU_TEX(x) ((x) << MPU_RASR_TEX_SHIFT)

/* Common Memory Type Combinations (TEX[2:0], C, B) */
#define MPU_ATTR_FLASH_ROM                                                     \
  (MPU_TEX(0) | MPU_RASR_C) /* TEX=000, C=1, B=0 - Normal, Non-cacheable */
#define MPU_ATTR_INTERNAL_SRAM                                                 \
  (MPU_TEX(0) | MPU_RASR_C |                                                   \
   MPU_RASR_B) /* TEX=000, C=1, B=1 - Normal, Write-back cacheable */
#define MPU_ATTR_EXTERNAL_RAM                                                  \
  (MPU_TEX(1) | MPU_RASR_C |                                                   \
   MPU_RASR_B) /* TEX=001, C=1, B=1 - Normal, Write-back cacheable */

/* Peripheral Memory Attributes - Platform-specific optimizations */
#define MPU_ATTR_PERIPHERAL_STRONGLY_ORDERED                                   \
  (MPU_TEX(0)) /* TEX=000, C=0, B=0 - Strongly-ordered for critical regs */
#define MPU_ATTR_PERIPHERAL_DEVICE                                             \
  (MPU_TEX(2)) /* TEX=010, C=0, B=0 - Device memory for normal peripherals */
#define MPU_ATTR_PERIPHERAL_DEVICE_XN                                          \
  (MPU_TEX(2) | MPU_RASR_XN) /* Device memory + Execute Never */

/* Legacy compatibility - use device memory for better performance */
#define MPU_ATTR_PERIPHERAL MPU_ATTR_PERIPHERAL_DEVICE
#define MPU_ATTR_PERIPHERAL_XN MPU_ATTR_PERIPHERAL_DEVICE_XN

/* Access Permission Helper Macros */
#define MPU_PERM_NO_ACCESS() MPU_AP_NO_ACCESS
#define MPU_PERM_PRIV_RW() MPU_AP_PRIV_RW
#define MPU_PERM_USER_RO() MPU_AP_PRIV_RW_USER_RO
#define MPU_PERM_ALL_RW() MPU_AP_ALL_RW
#define MPU_PERM_PRIV_RO() MPU_AP_PRIV_RO
#define MPU_PERM_ALL_RO() MPU_AP_ALL_RO

/* Combined Configuration Macros for Common Use Cases */
#define MPU_CONFIG_FLASH_CODE (MPU_ATTR_FLASH_ROM | MPU_PERM_ALL_RO())
#define MPU_CONFIG_SRAM_DATA                                                   \
  (MPU_ATTR_INTERNAL_SRAM | MPU_PERM_ALL_RW() | MPU_RASR_XN)
#define MPU_CONFIG_PERIPHERAL_RW (MPU_ATTR_PERIPHERAL_XN | MPU_PERM_PRIV_RW())
#define MPU_CONFIG_USER_STACK                                                  \
  (MPU_ATTR_INTERNAL_SRAM | MPU_PERM_ALL_RW() | MPU_RASR_XN)
#define MPU_CONFIG_KERNEL_DATA                                                 \
  (MPU_ATTR_INTERNAL_SRAM | MPU_PERM_PRIV_RW() | MPU_RASR_XN)

/* STM32F4-specific peripheral configurations for different memory access
 * patterns */
#define MPU_CONFIG_PERIPHERAL_CRITICAL                                         \
  (MPU_ATTR_PERIPHERAL_STRONGLY_ORDERED | MPU_PERM_PRIV_RW() | MPU_RASR_XN)
#define MPU_CONFIG_PERIPHERAL_DEVICE                                           \
  (MPU_ATTR_PERIPHERAL_DEVICE_XN | MPU_PERM_PRIV_RW())
#define MPU_CONFIG_CCM_SRAM                                                    \
  (MPU_ATTR_INTERNAL_SRAM | MPU_PERM_PRIV_RW() | MPU_RASR_XN)

/* MPU Size and Validation Helper Macros */
#define MPU_MIN_REGION_SIZE 32 /* Minimum MPU region size */
#define MPU_IS_POWER_OF_2(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))
#define MPU_IS_VALID_SIZE(x)                                                   \
  ((x) >= MPU_MIN_REGION_SIZE && MPU_IS_POWER_OF_2(x))
#define MPU_IS_ALIGNED(addr, size) (((addr) & ((size) - 1)) == 0)

/* Maximum number of MPU regions */
#define MPU_MAX_REGIONS CORTEX_M_MPU_MAX_REGIONS

/* MPU region configuration structure */
typedef struct {
  uint32_t base_addr;
  uint32_t size;
  uint32_t attributes;
  uint8_t region_id;
  uint8_t enabled;
} mpu_region_t;

/* Function prototypes */
pok_ret_t pok_mpu_init(void);
pok_ret_t pok_mpu_configure_region(uint8_t region, uint32_t base_addr,
                                   uint32_t size, uint32_t attributes);
pok_ret_t pok_mpu_configure_region_with_subregions(uint8_t region,
                                                   uint32_t base_addr,
                                                   uint32_t actual_size,
                                                   uint32_t aligned_size,
                                                   uint32_t attributes);
pok_ret_t pok_mpu_enable_region(uint8_t region);
pok_ret_t pok_mpu_disable_region(uint8_t region);
pok_ret_t pok_mpu_enable(void);
pok_ret_t pok_mpu_disable(void);
uint8_t pok_mpu_get_region_count(void);
uint32_t pok_mpu_size_to_rasr(uint32_t size);
uint8_t pok_mpu_get_active_user_region(void);
uint32_t pok_mpu_get_region_base(uint8_t region);

#endif /* !__POK_ARM_MPU_H__ */
