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
 * \file    arch/x86/space.c
 * \brief   Handle address spaces
 * \author  Julian Pidancet
 */

#include <types.h>
#include <errno.h>
#include <libc.h>
#include <bsp.h>

#include <arch.h>

#include <arch/x86/interrupt.h>

#include "gdt.h"
#include "tss.h"

#include "space.h"

#define KERNEL_STACK_SIZE 8192

pok_ret_t pok_create_space (uint8_t partition_id,
                            uint32_t addr,
                            uint32_t size)
{
   gdt_set_segment (GDT_PARTITION_CODE_SEGMENT (partition_id),
         addr, size, GDTE_CODE, 3);

   gdt_set_segment (GDT_PARTITION_DATA_SEGMENT (partition_id),
         addr, size, GDTE_DATA, 3);

   return (POK_ERRNO_OK);
}

pok_ret_t pok_space_switch (uint8_t old_partition_id,
                            uint8_t new_partition_id)
{
   gdt_disable (GDT_PARTITION_CODE_SEGMENT(old_partition_id));
   gdt_disable (GDT_PARTITION_DATA_SEGMENT(old_partition_id));
   gdt_enable (GDT_PARTITION_CODE_SEGMENT(new_partition_id));
   gdt_enable (GDT_PARTITION_DATA_SEGMENT(new_partition_id));

   return (POK_ERRNO_OK);
}

uint32_t	pok_space_base_vaddr (uint32_t addr)
{
   (void) addr;
   return (0);
}

uint32_t	pok_space_context_create (uint8_t  partition_id,
                                   uint32_t entry_rel,
                                   uint32_t stack_rel,
                                   uint32_t arg1,
                                   uint32_t arg2)
{
   char*             stack_addr;
   space_context_t*  sp;
   
   stack_addr = pok_bsp_mem_alloc (KERNEL_STACK_SIZE);

   sp = (space_context_t *)
      (stack_addr + KERNEL_STACK_SIZE - 4 - sizeof (space_context_t));

   memset (sp, 0, sizeof (space_context_t));

   sp->ctx.__esp  = (uint32_t)(&sp->ctx.eip); /* for pusha */
   sp->ctx.eip    = (uint32_t)pok_dispatch_space;
   sp->ctx.cs     = GDT_CORE_CODE_SEGMENT << 3;
   sp->ctx.eflags = 1 << 9;

   sp->arg1          = arg1;
   sp->arg2          = arg2;
   sp->kernel_sp     = (uint32_t)sp;
   sp->user_sp       = stack_rel;
   sp->user_pc       = entry_rel;
   sp->partition_id  = partition_id;

   return ((uint32_t) sp);
}

#ifdef POK_NEEDS_ERROR_HANDLING
void pok_space_context_restart (uint32_t sp, uint32_t entry, uint32_t user_stack)
{
   space_context_t* ct;

   ct = (space_context_t*) sp;
   ct->ctx.__esp  = (uint32_t)(&ct->ctx.eip); /* for pusha */
   ct->ctx.eip    = (uint32_t)pok_dispatch_space;
   ct->user_pc = entry;
   ct->user_sp = user_stack;
}
#endif

void pok_dispatch_space (uint8_t partition_id,
                         uint32_t user_pc,
                         uint32_t user_sp,
                         uint32_t kernel_sp,
                         uint32_t arg1,
                         uint32_t arg2)
{
   interrupt_frame   ctx;
   uint32_t          code_sel;
   uint32_t          data_sel;
   uint32_t          sp;

   code_sel = GDT_BUILD_SELECTOR (GDT_PARTITION_CODE_SEGMENT (partition_id), 0, 3);
   data_sel = GDT_BUILD_SELECTOR (GDT_PARTITION_DATA_SEGMENT (partition_id), 0, 3);

   sp = (uint32_t) &ctx;

   memset (&ctx, 0, sizeof (interrupt_frame));

   pok_arch_preempt_disable ();

   ctx.es = ctx.ds = ctx.ss = data_sel;

   ctx.__esp   = (uint32_t) (&ctx.error); /* for pusha */
   ctx.eip     = user_pc;
   ctx.eax     = arg1;
   ctx.ebx     = arg2;
   ctx.cs      = code_sel;
   ctx.eflags  = 1 << 9;
   ctx.esp     = user_sp;

   tss_set_esp0 (kernel_sp);

   asm ("mov %0, %%esp		\n"
        "pop %%es		\n"
        "pop %%ds		\n"
        "popa			\n"
        "addl $4, %%esp		\n"
        "iret			\n"
        :
        : "m" (sp)
       );
}
