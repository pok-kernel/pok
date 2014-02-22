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


#include <types.h>
#include <errno.h>
#include <libc.h>
#include <bsp.h>
#include <core/sched.h>

#include <arch.h>
#include "thread.h"
#include "msr.h"

#define KERNEL_STACK_SIZE 8192

#define PPC_SR_KP (1 << 29)
#define PPC_SR_Ks (1 << 30)
#define PPC_SR_T  (1 << 31)

struct pok_space
{
  uint32_t phys_base;
  uint32_t size;
};

struct pok_space spaces[POK_CONFIG_NB_PARTITIONS];

pok_ret_t pok_create_space (uint8_t partition_id,
                            uint32_t addr,
                            uint32_t size)
{
#ifdef POK_NEEDS_DEBUG
  printf ("pok_create_space: %d: %x %x\n", partition_id, addr, size);
#endif
  spaces[partition_id].phys_base = addr;
  spaces[partition_id].size = size;

  return (POK_ERRNO_OK);
}

pok_ret_t pok_space_switch (uint8_t old_partition_id,
                            uint8_t new_partition_id)
{
  (void) old_partition_id;
  /* printf ("space_switch %u -> %u\n", old_partition_id, new_partition_id); */
  asm volatile ("mtsr %0,%1" : : "r"(0), "r"(PPC_SR_KP | new_partition_id));
  return (POK_ERRNO_OK);
}

uint32_t	pok_space_base_vaddr (uint32_t addr)
{
   (void) addr;
   return (0);
}

extern void pok_arch_rfi (void);

uint32_t	pok_space_context_create (uint8_t partition_id,
                                   uint32_t entry_rel,
                                   uint32_t stack_rel,
                                   uint32_t arg1,
                                   uint32_t arg2)
{
  context_t* ctx;
  volatile_context_t* vctx;
  char*      stack_addr;
  (void) partition_id;

  stack_addr = pok_bsp_mem_alloc (KERNEL_STACK_SIZE);

  vctx = (volatile_context_t *)
    (stack_addr + KERNEL_STACK_SIZE - sizeof (volatile_context_t));
  ctx = (context_t *)((char *)vctx - sizeof (context_t) + 8);

  memset (ctx, 0, sizeof (*ctx));
  memset (vctx, 0, sizeof (*vctx));

  vctx->r3     = arg1;
  vctx->r4     = arg2;
  vctx->sp     = stack_rel - 12;
  vctx->srr0   = entry_rel;
  vctx->srr1   = MSR_EE | MSR_IP | MSR_DR | MSR_IR | MSR_PR;
  ctx->lr      = (uint32_t) pok_arch_rfi;

  ctx->sp      = (uint32_t) &vctx->sp;

#ifdef POK_NEEDS_DEBUG
  printf ("space_context_create %d: entry=%x stack=%x arg1=%x arg2=%x ksp=%x\n",
          partition_id, entry_rel, stack_rel, arg1, arg2, &vctx->sp);
#endif

  return (uint32_t)ctx;
}

typedef struct
{
  uint32_t vsid_api;
  uint32_t rpn_flags;
} ppc_pte_t;

static uint32_t pt_base;
static uint32_t pt_mask;

#define PPC_PTE_V (1 << 31)
#define POK_PAGE_SIZE (1 << 12)
#define POK_PAGE_MASK (~(POK_PAGE_SIZE - 1))
#define PPC_PTE_H (1 << 6)
#define PPC_PTE_R (1 << 8)
#define PPC_PTE_C (1 << 7)
#define PPC_PTE_W (1 << 6)
#define PPC_PTE_I (1 << 5)
#define PPC_PTE_M (1 << 4)
#define PPC_PTE_G (1 << 3)
#define PPC_PTE_PP_NO 0
#define PPC_PTE_PP_RO 1
#define PPC_PTE_PP_RW 2

void pok_arch_space_init (void)
{
  uint32_t sdr1;

  pt_base = 0;
  pt_mask = 0x3ff;

  sdr1 = pt_base | (pt_mask >> 10);
  asm volatile ("mtsdr1 %0" : : "r"(sdr1));
}

static void pok_insert_pte (uint32_t vsid, uint32_t vaddr, uint32_t pte)
{
  uint32_t hash = (vsid & 0x7ffff) ^ ((vaddr >> 12) & 0xffff);
  ppc_pte_t *pteg;
  int i;

  pteg = (ppc_pte_t *)(pt_base + ((hash & pt_mask) << 6));
  for (i = 0; i < 8; i++)
    if (!(pteg[i].vsid_api & PPC_PTE_V))
      {
        pteg[i].rpn_flags = pte;
        pteg[i].vsid_api = PPC_PTE_V | (vsid << 7) | ((vaddr >> 22) & 0x3f);
        return;
      }

  /* FIXME: Try secondary hash.  */

#ifdef POK_NEEDS_DEBUG
  printf ("pok_insert_pte: no free entry\n");
#endif

  while (1)
    ;
}

void pok_arch_isi_int (uint32_t pc, uint32_t msr)
{

#ifdef POK_NEEDS_DEBUG
  printf("isi_int: part=%d, pc=%x msr=%x\n",
         pok_current_partition, pc, msr);

  if (msr & ((1 << 28) | (1 << 27)))
  {
    printf (" Bad access\n");
  }
#endif

  if (msr & (1 << 30))
    {
      /* Page fault  */
      if (pc < spaces[pok_current_partition].size)
        {
          uint32_t vaddr = pc & POK_PAGE_MASK;
          uint32_t v;
          v = (spaces[pok_current_partition].phys_base + vaddr) & POK_PAGE_MASK;
          v |= PPC_PTE_R | PPC_PTE_C | PPC_PTE_PP_RW;
          pok_insert_pte (pok_current_partition, vaddr, v);
          return;
        }
    }

#ifdef POK_NEEDS_DEBUG
   printf("[DEBUG] Infinite loop in pok_arch_isi_int\n");
#endif

  while (1)
    ;
}

void pok_arch_dsi_int (uint32_t dar, uint32_t dsisr)
{
#ifdef POK_NEEDS_DEBUG
  printf("dsi_int: part=%d, dar=%x dsisr=%x\n",
         pok_current_partition, dar, dsisr);
#endif

  if (dsisr & (1 << 30))
    {
      /* Page fault  */
      if (dar < spaces[pok_current_partition].size)
        {
          uint32_t vaddr = dar & POK_PAGE_MASK;
          uint32_t v;
          v = (spaces[pok_current_partition].phys_base + vaddr) & POK_PAGE_MASK;
          v |= PPC_PTE_R | PPC_PTE_C | PPC_PTE_PP_RW;
          pok_insert_pte (pok_current_partition, vaddr, v);
          return;
        }
    }
#ifdef POK_NEEDS_DEBUG
   printf("[DEBUG] Infinite loop in pok_arch_dsi_int\n");
#endif
  while (1)
    ;
}
