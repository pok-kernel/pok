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

#ifndef __POK_ARM_SPACE_H__
#define __POK_ARM_SPACE_H__

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque type; definition lives in space.c */
struct pok_space;
typedef struct pok_space pok_space_t;

/* Public API (implemented in kernel/arch/arm/space.c) */
pok_ret_t pok_arch_space_init(void);
pok_ret_t pok_create_space(uint8_t partition_id, uint32_t addr, uint32_t size);
pok_ret_t pok_create_code_region(uint8_t partition_id, uint32_t code_addr,
                                 uint32_t code_size);
pok_ret_t pok_space_switch(uint8_t old_partition_id, uint8_t new_partition_id);
uint32_t pok_space_base_vaddr(uint32_t addr);
uint32_t pok_space_context_create(uint8_t partition_id, uint32_t entry_rel,
                                  uint8_t processor_affinity,
                                  uint32_t stack_rel, uint32_t arg1,
                                  uint32_t arg2);

/* Exported for inspection/debug if needed (size unspecified). */
extern pok_space_t spaces[];

#ifdef __cplusplus
}
#endif

#endif /* __POK_ARM_SPACE_H__ */
