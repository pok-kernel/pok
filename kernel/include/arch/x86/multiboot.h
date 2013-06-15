/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */

/**
 * \file    include/arch/x86/multiboot.h
 * \author  Julien Pidancet
 * \date    2008-2009
 */

#ifndef __POK_MULTIBOOT_H__
#define __POK_MULTIBOOT_H__

#define MULTIBOOT_BOOTLOADER_MAGIC     0x2BADB002

/**
 * The magic number for the Multiboot header.  
 */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/**
 * The flags for the Multiboot header.  
 * */
#define MULTIBOOT_HEADER_FLAGS          0x00010003

/**
 * The magic number passed by a Multiboot-compliant boot loader.
 */ 
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/**
 * The size of our stack (16KB).  
 */
#define MULTIBOOT_STACK_SIZE            0x4000

#define MULTIBOOT_CMDLINE 4
#define MULTIBOOT_MODS 8

/**
 * C symbol format. HAVE_ASM_USCORE is defined by configure.
 */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)                     _ ## sym
#else
# define EXT_C(sym)                     sym
#endif

#ifndef ASM
/* Do not include here in boot.S.  */


typedef struct
{
   unsigned int                  magic;
   unsigned int                  flags;
   unsigned int                  checksum;
   unsigned int                  header_addr;
   unsigned int                  load_addr;
   unsigned int                  load_end_addr;
   unsigned int                  bss_end_addr;
   unsigned int                  entry_addr;
} pok_multiboot_header_t;

typedef struct
{
   unsigned int                  tabsize;
   unsigned int                  strsize;
   unsigned int                  addr;
   unsigned int                  reserved;
} pok_aout_symbol_table_t;

typedef struct
{
   unsigned int                  num;
   unsigned int                  size;
   unsigned int                  addr;
   unsigned int                  shndx;
} pok_elf_section_header_table_t;

typedef struct
{
   unsigned int                  flags;
   unsigned int                  mem_lower;
   unsigned int                  mem_upper;
   unsigned int                  boot_device;
   unsigned int                  cmdline;
   unsigned int                  mods_count;
   unsigned int                  mods_addr;

   union
   {
      pok_aout_symbol_table_t         aout_sym;
      pok_elf_section_header_table_t  elf_sec;
   }                             u;

   unsigned int                  mmap_length;
   unsigned int                  mmap_addr;
} pok_multiboot_info_t;

typedef struct
{
   unsigned int                  mod_start;
   unsigned int                  mod_end;
   unsigned int                  string;
   unsigned int                  reserved;
} pok_module_t;

typedef struct
{
   unsigned int                  size;
   unsigned int                  base_addr_low;
   unsigned int                  base_addr_high;
   unsigned int                  length_low;
   unsigned int                  length_high;
   unsigned int                  type;
} pok_memory_map_t;

#endif

#endif /* __POK_MULTIBOOT_H__ */
