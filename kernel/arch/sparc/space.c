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
 * @file
 * @author Fabien Chouteau
 * @brief  Memory management in SPARC.
 */

#include <types.h>
#include <errno.h>
#include <libc.h>
#include <bsp.h>
#include <core/sched.h>

#include <arch.h>
#include "thread.h"
#include "space.h"
#include "sparc_conf.h"
#include "context_offset.h"
#include "ioports.h"

#define KERNEL_STACK_SIZE 8192

/**
 * Basic paritions informtation needed for memory managment.
 */
struct pok_space
{
    uint32_t phys_base; /**< Pysical adress of the partition. */
    uint32_t size; /**< Size of the partition. */
};

struct pok_space spaces[POK_CONFIG_NB_PARTITIONS];

/**
 * MMU contexts table. (cf SPARC V8 Manual, page 243)
 */
ptd mmu_contexts_tab[POK_CONFIG_NB_PARTITIONS]
__attribute__ ((aligned (POK_CONFIG_NB_PARTITIONS * sizeof (ptd))));

/**
 * MMU level 1 table. (cf SPARC V8 Manual, page 243)
 */
ptd mmu_level1_tab[POK_CONFIG_NB_PARTITIONS][MM_LVL1_ENTRIES_NBR]
__attribute__ ((aligned (MM_LVL1_ENTRIES_NBR * sizeof (ptd))));

/**
 * MMU level 2 table. (cf SPARC V8 Manual, page 243)
 */
pte mmu_level2_tab[POK_CONFIG_NB_PARTITIONS][MM_LVL2_ENTRIES_NBR]
__attribute__ ((aligned (MM_LVL2_ENTRIES_NBR * sizeof (pte))));

/**
 * Set ptd and pte for the given partition.
 */
pok_ret_t pok_create_space (uint8_t partition_id,
                            uint32_t addr,
                            uint32_t size)
{
  if (size > SPARC_PARTITION_SIZE)
  {
#ifdef POK_NEEDS_DEBUG
    printf ("pok_create_space: %d: partition size too big 0x%x\n", partition_id, size);
#endif
    return (POK_ERRNO_SIZE);
  }

  if ((addr & (SPARC_PAGE_SIZE - 1)) != 0)
  {
#ifdef POK_NEEDS_DEBUG
    printf ("pok_create_space: %d: partition address not aligned 0x%x\n", partition_id, addr);
#endif
    return (POK_ERRNO_EFAULT);
  }
#ifdef POK_NEEDS_DEBUG
  printf ("pok_create_space: %d: %x %x\n", partition_id, addr, size);
#endif
  spaces[partition_id].phys_base = addr;
  spaces[partition_id].size = size;

  unsigned int as_ptd = mm_index1(SPARC_PARTITION_BASE_VADDR);
  unsigned int as_pte = mm_index2(SPARC_PARTITION_BASE_VADDR);

  mmu_level1_tab[partition_id][as_ptd] = ((unsigned int) &(mmu_level2_tab[partition_id]) >> 4) | MM_ET_PTD;
  /* partition as */
  mmu_level2_tab[partition_id][as_pte] = ((addr) >> 4) | MM_ACC_RWE | MM_ET_PTE | MM_CACHEABLE;

  return (POK_ERRNO_OK);
}

/**
 * Switch adress space in MMU (context register).
 */
pok_ret_t pok_space_switch (uint8_t old_partition_id,
                            uint8_t new_partition_id)
{
  (void) old_partition_id;

  asm volatile ("flush\n"
                "sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (new_partition_id), "r" (MMU_CTX_REG), "i" (ASI_M_MMUREGS)
                : "memory");
  return (POK_ERRNO_OK);
}

/**
 * @return partition virtual base adress.
 * @see SPARC_PARTITION_BASE_VADDR
 */
uint32_t pok_space_base_vaddr (uint32_t addr)
{
   (void) addr;
   return (SPARC_PARTITION_BASE_VADDR);
}

/**
 * Initilize thread stack.
 */
uint32_t pok_space_context_create (uint8_t  id,
                                   uint32_t entry_rel,
                                   uint32_t stack_rel,
                                   uint32_t arg1,
                                   uint32_t arg2)
{
  uint32_t ctx = spaces[id].phys_base + stack_rel - 0x40;

  outw(ctx - RESTORE_CNT_OFFSET, 1); /* Only 1 register window needed */
  outw(ctx - PC_OFFSET, entry_rel);
  outw(ctx - NPC_OFFSET, entry_rel + 4);
  outw(ctx - I0_OFFSET, arg1);
  outw(ctx - I1_OFFSET, arg2);

#ifdef POK_NEEDS_DEBUG
  printf ("space_context_create part_id=%d entry=%x stack=%x arg1=%x arg2=%x\n",
          id, entry_rel, stack_rel, arg1, arg2);
#endif

  return SPARC_PARTITION_BASE_VADDR + stack_rel - 0x40;
}

/**
 * Initilize MMU tables.
 */
void pok_arch_space_init (void)
{
  int i = 0;
  int j = 0;

  for (i = 0; i < POK_CONFIG_NB_PARTITIONS; i++)
    mmu_contexts_tab[i] = MM_ET_INVALID;

  for (i = 0; i < POK_CONFIG_NB_PARTITIONS; i++)
  {
    mmu_contexts_tab[i] = (unsigned int)&(mmu_level1_tab[i]) >> 4 | MM_ET_PTD;

    for (j = 0; j < MM_LVL1_ENTRIES_NBR; j++)
    {
      mmu_level1_tab[i][j] = MM_ET_INVALID;
    }

    for (j = 0; j < MM_LVL2_ENTRIES_NBR; j++)
    {
      mmu_level2_tab[i][j] = MM_ET_INVALID;
    }
  }

  unsigned int kernel_pte = mm_index1(SPARC_RAM_ADDR);

  /* the kernel code is always mapped on a 16Mb page (including all partitions) */
  for (i = 0; i < POK_CONFIG_NB_PARTITIONS; i++)
  {
    mmu_level1_tab[i][kernel_pte] = (SPARC_RAM_ADDR >> 4) | MM_ACC_S_RWE | MM_ET_PTE | MM_CACHEABLE;
  }


  /* set context table */
  asm volatile ("sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (((unsigned int) mmu_contexts_tab) >> 4), "r" (MMU_CTXTBL_PTR), "i" (ASI_M_MMUREGS)
                : "memory");

  /* set context number */
  pok_space_switch(0, 0);

  asm volatile ("flush\n"
                "sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (0x1), "r" (MMU_CTRL_REG), "i" (ASI_M_MMUREGS)
                : "memory");


#ifdef POK_NEEDS_DEBUG
    printf ("pok_arch_space_init: ctx nbr=%u\n", POK_CONFIG_NB_PARTITIONS);
#endif
}
