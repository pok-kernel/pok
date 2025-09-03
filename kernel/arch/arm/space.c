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
 * \file    arch/arm/space.c
 * \brief   ARM Cortex-M address space management using MPU
 * \author  POK team
 */

/* POK system headers */
#include <errno.h>
#include <libc.h>
#include <types.h>

/* POK core headers */
#include <bsp.h>
#include <core/sched.h>

/* Architecture-specific headers */
#include "arch.h"
#include "cortex_m_config.h"
#include "mpu.h"
#include "mpu_utils.h"
#include "thread.h"

#define KERNEL_STACK_SIZE 4096
#define MEMORY_WASTE_THRESHOLD_PERCENT 25
#define MEMORY_WASTE_CRITICAL_PERCENT                                          \
  50 /* Fail allocation if waste exceeds this */
#define STACK_ALIGNMENT_BYTES                                                  \
  CORTEX_M_STACK_ALIGNMENT /* ARM Cortex-M requires 8-byte stack alignment */
#define STACK_ALIGNMENT_MASK CORTEX_M_STACK_ALIGNMENT_MASK

/* Helper function to align address down while keeping it within bounds */
static inline uint32_t align_down(uint32_t value, uint32_t alignment) {
  return value & ~(alignment - 1);
}

/* Partition space information */
struct pok_space {
  uint32_t phys_base;
  uint32_t size;
  uint8_t mpu_region;
  uint8_t mpu_code_region; /* Separate code region for W^X enforcement */
  uint32_t code_base;      /* Base address of code region */
  uint32_t code_size;      /* Size of code region */
};

struct pok_space spaces[POK_CONFIG_NB_PARTITIONS];

/**
 * Create a memory space for a partition using MPU protection
 *
 * @param partition_id ID of the partition (0 to POK_CONFIG_NB_PARTITIONS-1)
 * @param addr Base address of the partition memory space
 * @param size Size of the partition memory space
 * @return POK_ERRNO_OK on success, error code on failure
 */
pok_ret_t pok_create_space(uint8_t partition_id, uint32_t addr, uint32_t size) {
  uint32_t mpu_attributes;
  uint8_t region_id;

  if (partition_id >= POK_CONFIG_NB_PARTITIONS) {
    return POK_ERRNO_EINVAL;
  }

  /* Validate nonzero/valid size early to prevent division by zero */
  if (size == 0) {
    return POK_ERRNO_EINVAL; /* Zero size is invalid */
  }

  /* Use partition_id + 1 as region ID (reserve region 0 for kernel) */
  region_id = partition_id + 1;

  if (region_id >= pok_mpu_get_region_count()) {
    return POK_ERRNO_EINVAL;
  }

  /* Configure MPU attributes for partition space - enforce W^X principle:
   * - Read/Write access for data region (no execute)
   * - Read/Execute access for code region (no write)
   * - Normal memory with caching
   * Note: This creates a data region first, code region will be separate
   */
  /* Use helper macro for partition data region (read-write, no execute) */
  mpu_attributes = MPU_CONFIG_SRAM_DATA;

  /* Align size to power of 2 (MPU requirement) */
  uint32_t aligned_size = mpu_align_size_to_power_of_2(size);

  /* Validate base address alignment matches MPU requirements */
  if (!mpu_is_aligned(addr, aligned_size)) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Partition %d base addr 0x%x not aligned to size 0x%x\n",
           partition_id, addr, aligned_size);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Configure MPU region - try subregions first to minimize waste */
  uint32_t actual_exposed_memory = aligned_size - size;
  pok_bool_t used_subregions = FALSE;
  uint32_t configured_region_id = region_id; /* Track for rollback */

  if (aligned_size >= ARM_MPU_MIN_SUBREGION_SIZE &&
      (aligned_size - size) >= (size / 4)) {
    /* Use subregions if region is large enough and waste is significant */
    if (pok_mpu_configure_region_with_subregions(
            region_id, addr, size, aligned_size, mpu_attributes) ==
        POK_ERRNO_OK) {
      used_subregions = TRUE;
      /* Calculate real exposed memory with subregions - subregions can still
       * expose unused memory at boundaries */
      uint32_t subregion_size = aligned_size / 8; /* MPU has 8 subregions */
      uint32_t needed_subregions = (size + subregion_size - 1) / subregion_size;
      actual_exposed_memory = (needed_subregions * subregion_size) - size;
    }
  }

  if (!used_subregions) {
    /* Use standard region configuration */
    if (pok_mpu_configure_region(region_id, addr, aligned_size,
                                 mpu_attributes) != POK_ERRNO_OK) {
      return POK_ERRNO_EFAULT;
    }
    /* Standard region exposes full aligned_size - size */
    actual_exposed_memory = aligned_size - size;
  }

  /* Check waste after subregion masking */
  if (actual_exposed_memory > 0) {
    uint32_t waste_percent = (actual_exposed_memory * 100) / size;
    if (waste_percent > MEMORY_WASTE_CRITICAL_PERCENT) {
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: Partition %d wastes %u%% memory (%u bytes) "
             "- exceeds %u%% limit\n",
             partition_id, waste_percent, actual_exposed_memory,
             MEMORY_WASTE_CRITICAL_PERCENT);
#endif
      /* Rollback: disable the configured MPU region */
      pok_mpu_disable_region(configured_region_id);
      return POK_ERRNO_EINVAL;
    }

#ifdef POK_NEEDS_DEBUG
    if (waste_percent > MEMORY_WASTE_THRESHOLD_PERCENT) {
      printf("WARNING: Partition %d wastes %u%% memory (%u bytes) "
             "- protected by MPU hardware\n",
             partition_id, waste_percent, actual_exposed_memory);
    }
#endif
  }

  /* Store partition information */
  spaces[partition_id].phys_base = addr;
  spaces[partition_id].size = size;
  spaces[partition_id].mpu_region = region_id;
  spaces[partition_id].mpu_code_region = 0; /* No code region yet */
  spaces[partition_id].code_base = 0;
  spaces[partition_id].code_size = 0;

  /* Initially disable the region */
  pok_mpu_disable_region(region_id);

#ifdef POK_NEEDS_DEBUG
  printf("pok_create_space: %d: %x %x (region %d)\n", partition_id, addr, size,
         region_id);
#endif

  return POK_ERRNO_OK;
}

/**
 * Create a separate code region for a partition to enforce W^X security
 *
 * @param partition_id ID of the partition
 * @param code_addr Base address of the code region within partition
 * @param code_size Size of the code region
 * @return POK_ERRNO_OK on success, error code on failure
 */
pok_ret_t pok_create_code_region(uint8_t partition_id, uint32_t code_addr,
                                 uint32_t code_size) {
  uint32_t mpu_attributes;
  uint8_t region_id;

  if (partition_id >= POK_CONFIG_NB_PARTITIONS) {
    return POK_ERRNO_EINVAL;
  }

  /* Require existing partition before creating code region */
  if (spaces[partition_id].size == 0) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Cannot create code region for non-existent partition %d\n",
           partition_id);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Use partition_id + 1 + POK_CONFIG_NB_PARTITIONS as code region ID */
  region_id = partition_id + 1 + POK_CONFIG_NB_PARTITIONS;

  /* Region budget check: Ensure we don't exceed available MPU regions
   * Budget allocation:
   * - Region 0: Reserved for kernel
   * - Regions 1 to POK_CONFIG_NB_PARTITIONS: Partition data regions
   * - Regions (POK_CONFIG_NB_PARTITIONS+1) to (2*POK_CONFIG_NB_PARTITIONS):
   * Partition code regions Total needed: 1 + (2 * POK_CONFIG_NB_PARTITIONS)
   */
  uint32_t total_regions_needed = 1 + (2 * POK_CONFIG_NB_PARTITIONS);
  uint32_t available_regions = pok_mpu_get_region_count();

  if (total_regions_needed > available_regions) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: MPU region budget exceeded. Need %u regions, have %u\n",
           total_regions_needed, available_regions);
    printf("Reduce POK_CONFIG_NB_PARTITIONS or use MPU with more regions\n");
#endif
    return POK_ERRNO_EINVAL;
  }

  if (region_id >= available_regions) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Code region ID %u exceeds available regions %u\n", region_id,
           available_regions);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Enforce code region is inside the partition bounds with overflow-safe math
   */
  if (code_size == 0) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Code region size cannot be zero for partition %d\n",
           partition_id);
#endif
    return POK_ERRNO_EINVAL;
  }

  uint32_t partition_base = spaces[partition_id].phys_base;

  if (code_addr < partition_base) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Code region start 0x%x before partition bounds [0x%x-0x%x) "
           "for partition %d\n",
           code_addr, partition_base,
           partition_base + spaces[partition_id].size, partition_id);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Use offset math to avoid overflow in end address calculation */
  uint32_t code_offset = code_addr - partition_base;
  if (code_offset + code_size > spaces[partition_id].size) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Code region [0x%x+%u) extends beyond partition bounds "
           "[0x%x-0x%x) "
           "for partition %d\n",
           code_addr, code_size, partition_base,
           partition_base + spaces[partition_id].size, partition_id);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Configure MPU attributes for code region - enforce W^X:
   * - Read/Execute only (no write permission)
   * - Normal memory with caching
   */
  /* Use helper macro for code region (read-only, executable) */
  mpu_attributes = MPU_CONFIG_FLASH_CODE;

  /* Align size to power of 2 (MPU requirement) */
  uint32_t aligned_size = mpu_align_size_to_power_of_2(code_size);

  /* Validate base address alignment matches MPU requirements */
  if (!mpu_is_aligned(code_addr, aligned_size)) {
#ifdef POK_NEEDS_DEBUG
    printf(
        "ERROR: Partition %d code region addr 0x%x not aligned to size 0x%x\n",
        partition_id, code_addr, aligned_size);
#endif
    return POK_ERRNO_EINVAL;
  }

  /* Configure MPU region for partition code */
  if (pok_mpu_configure_region(region_id, code_addr, aligned_size,
                               mpu_attributes) != POK_ERRNO_OK) {
    return POK_ERRNO_EFAULT;
  }

  /* Store code region information for space switching */
  spaces[partition_id].mpu_code_region = region_id;
  spaces[partition_id].code_base = code_addr;
  spaces[partition_id].code_size = code_size;

  /* Initially disable the code region */
  pok_mpu_disable_region(region_id);

#ifdef POK_NEEDS_DEBUG
  printf("pok_create_code_region: partition %d: code addr=0x%x size=0x%x "
         "(region %d)\n",
         partition_id, code_addr, code_size, region_id);
#endif

  return POK_ERRNO_OK;
}

pok_ret_t pok_space_switch(uint8_t old_partition_id, uint8_t new_partition_id) {
  if (old_partition_id < POK_CONFIG_NB_PARTITIONS) {
    /* Disable old partition's data MPU region if valid (avoid affecting kernel
     * region) */
    if (spaces[old_partition_id].mpu_region != 0) {
      pok_mpu_disable_region(spaces[old_partition_id].mpu_region);
    }

    /* Disable old partition's code region if it exists */
    if (spaces[old_partition_id].mpu_code_region != 0) {
      pok_mpu_disable_region(spaces[old_partition_id].mpu_code_region);
    }
  }

  if (new_partition_id < POK_CONFIG_NB_PARTITIONS) {
    /* Enable new partition's data MPU region if valid (avoid affecting kernel
     * region) */
    if (spaces[new_partition_id].mpu_region != 0) {
      pok_mpu_enable_region(spaces[new_partition_id].mpu_region);
    }

    /* Enable new partition's code region if it exists (enforces W^X) */
    if (spaces[new_partition_id].mpu_code_region != 0) {
      pok_mpu_enable_region(spaces[new_partition_id].mpu_code_region);
    }
  }

  return POK_ERRNO_OK;
}

uint32_t pok_space_base_vaddr(uint32_t addr) {
  /* ARM Cortex-M uses flat memory model - no virtual addressing */
  return (addr);
}

uint32_t pok_space_context_create(uint8_t partition_id, uint32_t entry_rel,
                                  uint8_t processor_affinity,
                                  uint32_t stack_rel, uint32_t arg1,
                                  uint32_t arg2) {
  context_t *ctx;
  uint32_t entry_abs, stack_abs;

  if (partition_id >= POK_CONFIG_NB_PARTITIONS) {
    return (0);
  }

  /* ARM Cortex-M is single-core, ignore processor_affinity but validate it */
  (void)processor_affinity; /* Suppress unused parameter warning */

  /* Ensure partition was created */
  if (spaces[partition_id].size == 0) {
    return (0);
  }

  /* Bounds validation for entry and stack offsets */
  /* If partition has a separate code region, validate entry against code region
   * bounds */
  if (spaces[partition_id].mpu_code_region != 0) {
    /* Entry point must be within the code region */
    uint32_t code_region_base =
        spaces[partition_id].code_base - spaces[partition_id].phys_base;

    /* Avoid unsigned underflow when computing code_offset */
    if (entry_rel < code_region_base) {
      /* entry_rel is before code region - invalid */
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: Entry offset 0x%x before code region start 0x%x "
             "in partition %d\n",
             entry_rel, code_region_base, partition_id);
#endif
      return (0);
    }

    uint32_t code_offset = entry_rel - code_region_base;
    if (code_offset >= spaces[partition_id].code_size) {
#ifdef POK_NEEDS_DEBUG
      printf("ERROR: Entry offset 0x%x not within code region bounds "
             "(0x%x-0x%x) in partition %d\n",
             entry_rel, code_region_base,
             code_region_base + spaces[partition_id].code_size, partition_id);
#endif
      return (0);
    }
  } else {
    /* W^X consistency: require a code region or execution will HardFault */
#ifdef POK_NEEDS_DEBUG
    printf(
        "ERROR: No code region defined for partition %d - execution will fail "
        "due to W^X enforcement (data region is non-executable)\n",
        partition_id);
#endif
    return (0);
  }

  if (stack_rel >= spaces[partition_id].size) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Stack offset 0x%x exceeds partition %d size 0x%x\n",
           stack_rel, partition_id, spaces[partition_id].size);
#endif
    return (0);
  }

  /* Validate that stack has reasonable space (at least 1KB) */
  uint32_t remaining_stack_space = spaces[partition_id].size - stack_rel;
  if (remaining_stack_space < 1024) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Insufficient stack space %u bytes in partition %d\n",
           remaining_stack_space, partition_id);
#endif
    return (0);
  }

  /* Calculate absolute addresses after bounds validation */
  entry_abs = spaces[partition_id].phys_base + entry_rel;
  stack_abs = spaces[partition_id].phys_base + stack_rel;

  /* Validate aligned SP is within partition bounds */
  uint32_t sp_aligned = align_down(stack_abs, STACK_ALIGNMENT_BYTES);
  uint32_t base = spaces[partition_id].phys_base;
  uint32_t end = base + spaces[partition_id].size;
  if (sp_aligned < base || sp_aligned >= end) {
    return (0);
  }

  /* Create context frame at top of user stack (no kernel stack allocation) */
  ctx = (context_t *)(sp_aligned - sizeof(context_t));

  /* Validate context frame is still within partition bounds */
  if ((uint32_t)ctx < base || ((uint32_t)ctx + sizeof(context_t)) > end) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Context frame outside partition bounds in partition %d\n",
           partition_id);
#endif
    return (0);
  }

  /* Initialize context frame (zero-initialize first) */
  memset(ctx, 0, sizeof(context_t));

  /* Initialize ARM Cortex-M context for thread startup */
  ctx->r0 = arg1;                      /* First argument */
  ctx->r1 = arg2;                      /* Second argument */
  ctx->lr = ARM_EXC_RETURN_THREAD_PSP; /* Return to Thread mode, use PSP */
  ctx->pc = entry_abs | 1;             /* Entry point with Thumb bit set */
  ctx->xpsr = 0x01000000;              /* Thumb bit set */

#ifdef POK_NEEDS_DEBUG
  printf("space_context_create %d: entry=%x stack=%x arg1=%x arg2=%x ctx=%x\n",
         partition_id, entry_abs, stack_abs, arg1, arg2, (uint32_t)ctx);
#endif

  return (uint32_t)ctx;
}

pok_ret_t pok_arch_space_init(void) {
  pok_ret_t ret;

  /* Initialize partition spaces array */
  memset(spaces, 0, sizeof(spaces));

  /* Reserve region 0 for kernel space */
  /* Use helper macro for kernel data region */
  uint32_t kernel_attrs = MPU_CONFIG_KERNEL_DATA;
  ret = pok_mpu_configure_region(0, pok_bsp_kernel_base(),
                                 pok_bsp_kernel_size(), kernel_attrs);
  if (ret != POK_ERRNO_OK) {
#ifdef POK_NEEDS_DEBUG
    printf("ERROR: Failed to configure kernel MPU region: %d\n", ret);
#endif
    return ret;
  }

#ifdef POK_NEEDS_DEBUG
  printf("pok_arch_space_init: MPU regions=%d\n", pok_mpu_get_region_count());
#endif

  return POK_ERRNO_OK;
}
