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
 *                                      Copyright (c) 2007-2020 POK team
 */

/**
 * \file    include/arch/x86/multiprocessing.h
 * \author  Romain Guilloteau
 * \brief   Check if the system is multiprocessors and configure other
 * processors in this case
 */

#ifndef __POK_X86_MULTIPROCESSING__
#define __POK_X86_MULTIPROCESSING__

#include <types.h>

#define BDA ((unsigned char *)0x400)
#define BIOS ((unsigned char *)0xF0000)
#define READ_BIT(src, bit) (((src) & (1 << (bit))) >> (bit))
#define SET_BIT(dst, bit) ((dst) |= (1 << (bit)))

typedef struct {
  char signature[4];
  uint16_t base_table_length;
  uint8_t spec_rev;
  uint8_t checksum;
  uint64_t oem_id;
  uint8_t product_id[12];
  uint32_t oem_table_pointer;
  uint16_t oem_table_size;
  uint16_t entry_count;
  uint32_t lapic_addr;
  uint16_t extended_table_length;
  uint8_t extended_table_checksum;
  uint8_t __reserved__;

} __attribute__((__packed__)) mp_conf_table_header;

typedef struct {
  char signature[4];
  mp_conf_table_header *conf_table;
  uint8_t length;
  uint8_t spec_rev;
  uint8_t checksum;
  uint8_t mp_feat_1;
  uint8_t mp_feat_2;
  uint8_t mp_feat_3[3];
} __attribute__((__packed__)) mp_floating;

typedef struct {
  uint8_t entry_type;
  uint8_t lapic_id;
  uint8_t lapic_version;
  uint8_t cpu_flags;
  uint8_t cpu_signature[2];
  uint32_t feature_flags;
} __attribute__((__packed__)) processor_entry;

typedef struct {
  uint8_t entry_type;
  uint8_t id;
  uint8_t version;
  uint8_t enable;
  uint32_t address;
} __attribute__((__packed__)) io_apic_entry;

void pok_multiprocessing_init(void);

#endif /* __POK_X86_MULTIPROCESSING__ */
