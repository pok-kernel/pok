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
 **\file    thread.c
 **\brief   x86-dependent code for thread management
 **\author  Julian Pidancet
 */

#include <bsp.h>
#include <libc.h>
#include <errno.h>
#include <core/thread.h>

#include "gdt.h"

#include "thread.h"

#ifdef POK_NEEDS_THREADS

uint32_t		pok_context_create (uint32_t thread_id,
                                uint32_t stack_size,
                                uint32_t entry)
{
  start_context_t* sp;
  char*            stack_addr;

  stack_addr = pok_bsp_mem_alloc (stack_size);

  sp = (start_context_t *) (stack_addr + stack_size - 4 - sizeof (start_context_t));

  memset (sp, 0, sizeof (start_context_t));

  sp->ctx.__esp   = (uint32_t) (&sp->ctx.eip); /* for pusha */
  sp->ctx.eip     = (uint32_t) pok_thread_start;
  sp->ctx.cs      = GDT_CORE_CODE_SEGMENT << 3;
  sp->ctx.eflags  = 1 << 9;

  sp->entry       = entry;
  sp->id          = thread_id;

  return ((uint32_t)sp);
}


void			pok_context_switch (uint32_t* old_sp,
                                uint32_t new_sp);
asm (".global pok_context_switch	\n"
     "pok_context_switch:		\n"
     "pushf				\n"
     "pushl %cs				\n"
     "pushl $1f				\n"
     "pusha				\n"
     "movl 48(%esp), %ebx		\n" /* 48(%esp) : &old_sp, 52(%esp) : new_sp */
     "movl %esp, (%ebx)			\n"
     "movl 52(%esp), %esp		\n"
     "popa				\n"
     "iret				\n"
     "1:				\n"
     "ret"
     );

void			pok_context_reset(uint32_t stack_size,
					  uint32_t stack_addr)
{
  start_context_t* sp;
  uint32_t id;
  uint32_t entry;
  
  sp = (start_context_t *) (stack_addr + stack_size - 4 - sizeof (start_context_t));

  id = sp->id;
  entry = sp->entry;
  memset (sp, 0, sizeof (start_context_t));
  sp->ctx.__esp   = (uint32_t) (&sp->ctx.eip);
  sp->ctx.eip     = (uint32_t) pok_thread_start;
  sp->ctx.cs      = GDT_CORE_CODE_SEGMENT << 3;
  sp->ctx.eflags  = 1 << 9;
  sp->entry       = entry;
  sp->id          = id;
}

#endif
