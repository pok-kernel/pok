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
 * \file    x86/x86-qemu/pm.c
 * \author  Julian Pidancet
 * \author  Julien Delange
 * \date    2008-2009
 */

#include <errno.h>
#include <arch/x86/multiboot.h>
#include <types.h>

#include "pm.h"

#define ALIGN_UP(boundary, val) \
	(val + (boundary - 1)) & (~(boundary - 1))

extern void *__pok_begin;
extern void *__pok_end;

extern uint32_t pok_multiboot_magic;
extern uint32_t pok_multiboot_info;

uint32_t pok_x86_pm_heap_start;
uint32_t pok_x86_pm_brk;
uint32_t pok_x86_pm_heap_end;


int pok_pm_init ()
{
  pok_multiboot_info_t* mbi;
  uint32_t              free_mem;

  mbi = (pok_multiboot_info_t*) pok_multiboot_info;

#ifdef POK_NEEDS_DMA
  free_mem = MEM_16MB;
#else
  free_mem = ALIGN_UP (4096, (uint32_t)(&__pok_end));
#endif

  pok_x86_pm_heap_start = pok_x86_pm_brk = free_mem;

  pok_x86_pm_heap_end = (uint32_t)(mbi->mem_upper * 1024);

  return (POK_ERRNO_OK);
}

/**
 * Allocation function, very basic, just allocate
 * new memory space each time
 */
uint32_t pok_pm_sbrk (uint32_t increment)
{
  uint32_t addr;
  
  addr = pok_x86_pm_brk;

  pok_x86_pm_brk += increment;

  return (addr);
}

