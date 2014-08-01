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
 * \file    arch/x86/arch.c
 * \author  Julian Pidancet
 * \author  Julien Delange
 * \brief   Provides generic architecture interface for x86 architecture
 */

#include <errno.h>
#include <core/partition.h>

#include "event.h"
#include "gdt.h"

pok_ret_t pok_arch_init ()
{
  pok_gdt_init ();
  pok_event_init ();

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_disable()
{
  asm ("cli");
  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_enable()
{
  asm ("sti");
  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_idle()
{
   while (1)
   {
      asm ("hlt");
   }

   return (POK_ERRNO_OK);	
}


#ifdef POK_NEEDS_X86_VMM
//extern void pok_irq_prologue_0(void);
//extern void pok_irq_prologue_1(void);
//extern void pok_irq_prologue_2(void);
//extern void pok_irq_prologue_3(void);
//extern void pok_irq_prologue_4(void);
//extern void pok_irq_prologue_5(void);
//extern void pok_irq_prologue_6(void);
//extern void pok_irq_prologue_7(void);
//extern void pok_irq_prologue_8(void);
//extern void pok_irq_prologue_9(void);
//extern void pok_irq_prologue_10(void);
//extern void pok_irq_prologue_11(void);
//extern void pok_irq_prologue_12(void);
//extern void pok_irq_prologue_13(void);
//extern void pok_irq_prologue_14(void);
//extern void pok_irq_prologue_15(void);
//
//static void (*pok_irq_prologue[16])(void) = { 
//  pok_irq_prologue_0, 
//  pok_irq_prologue_1,
//  pok_irq_prologue_2,
//  pok_irq_prologue_3,
//  pok_irq_prologue_4,
//  pok_irq_prologue_5,
//  pok_irq_prologue_6,
//  pok_irq_prologue_7,
//  pok_irq_prologue_8,
//  pok_irq_prologue_9,
//  pok_irq_prologue_10,
//  pok_irq_prologue_11,
//  pok_irq_prologue_12,
//  pok_irq_prologue_13,
//  pok_irq_prologue_14,
//  pok_irq_prologue_15
//};

pok_ret_t pok_arch_event_register  (uint8_t vector,
                                    void (*handler)(void))
{
//  if( vector > 31 && vector < 48 ) /* first 32 lines reserved by intel */
//  {
//    pok_idt_set_gate (vector,
//                     GDT_CORE_CODE_SEGMENT << 3,
//		     (uint32_t) pok_irq_prologue[vector - 32], /* to fit the prologue array */
//                     IDTE_TRAP,
//                     3);
//
//    return (POK_ERRNO_OK);
//  }
//  else
  {
    pok_idt_set_gate (vector,
                     GDT_CORE_CODE_SEGMENT << 3,
		     (uint32_t) handler,
                     IDTE_TRAP,
                     3);

    return (POK_ERRNO_OK);
  }
}

#else
pok_ret_t pok_arch_event_register  (uint8_t vector,
                                    void (*handler)(void))
{
  pok_idt_set_gate (vector,
                   GDT_CORE_CODE_SEGMENT << 3,
      	     (uint32_t) handler,
                   IDTE_TRAP,
                   3);

  return (POK_ERRNO_OK);
}
#endif /* POK_NEEDS_X86_VMM */

uint32_t    pok_thread_stack_addr   (const uint8_t    partition_id,
                                     const uint32_t   local_thread_id)
{
   return pok_partitions[partition_id].size - 4 - (local_thread_id * POK_USER_STACK_SIZE);
}

