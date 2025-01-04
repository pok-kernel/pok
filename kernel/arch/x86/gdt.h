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

#ifndef __POK_X86_GDT_H__
#define __POK_X86_GDT_H__

#include <errno.h>
#include <types.h>

typedef enum e_gdte_type {
  GDTE_CODE = 0xB,
  GDTE_DATA = 0x3,
  GDTE_TSS = 0x9
} e_gdte_type;

typedef struct {
  uint32_t limit_low : 16;
  uint32_t base_low : 24 __attribute__((packed));
  uint32_t type : 4;
  uint32_t s : 1;
  uint32_t dpl : 2;
  uint32_t present : 1;
  uint32_t limit_high : 4;
  uint32_t available : 2;
  uint32_t op_size : 1;
  uint32_t granularity : 1;
  uint32_t base_high : 8;
} __attribute__((packed)) gdt_entry_t;

// POK uses the following GDT entries:
//   1: kernel code segment
//   2: kernel data segment
//   3: TSS
//   4 + 2n: partition n code segment
//   5 + 2n: partition n data segment
//
// Segments for partitions are enabled when the partition is scheduled (present
// = 1) and disabled otherwise. Kernel segments stay enable al the time.

#define GDT_CORE_CODE_SEGMENT 1
#define GDT_CORE_DATA_SEGMENT 2

#define GDT_TSS_SEGMENT(proc_id) (3 + (proc_id))
#define GDT_PARTITION_CODE_SEGMENT(partition_id)                               \
  (3 + POK_CONFIG_NB_PROCESSORS + 2 * partition_id)
#define GDT_PARTITION_DATA_SEGMENT(partition_id)                               \
  (3 + POK_CONFIG_NB_PROCESSORS + 2 * partition_id + 1)

#define GDT_BUILD_SELECTOR(seg, local, rpl)                                    \
  ((seg << 3) | ((local & 0x1) << 2) | (rpl & 0x3))

pok_ret_t pok_gdt_init(uint8_t proc);
int pok_tss_init(uint8_t proc);

void tss_set_esp0(uint8_t proc, uint32_t esp0);

void gdt_set_segment(uint16_t index, uint32_t base_address, uint32_t limit,
                     e_gdte_type t, int dpl);

void gdt_set_system(uint16_t index, uint32_t base_address, uint32_t limit,
                    e_gdte_type t, int dpl);

void gdt_enable(uint16_t index);
void gdt_disable(uint16_t index);

/*
 * DEPRECATED
uint32_t	gdt_segment_base(uint16_t idx);
*/

#endif /* !__POK_X86_GDT_H__ */
