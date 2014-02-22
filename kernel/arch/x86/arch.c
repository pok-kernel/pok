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

pok_ret_t pok_arch_event_register  (uint8_t vector,
                                    void (*handler)(void))
{
   pok_idt_set_gate (vector,
                     GDT_CORE_CODE_SEGMENT << 3,
                     (uint32_t)handler,
                     IDTE_TRAP,
                     3);

   return (POK_ERRNO_OK);
}

uint32_t    pok_thread_stack_addr   (const uint8_t    partition_id,
                                     const uint32_t   local_thread_id)
{
   return pok_partitions[partition_id].size - 4 - (local_thread_id * POK_USER_STACK_SIZE);
}

