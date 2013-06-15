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


#ifndef __POK_X86_THREAD_H__
#define __POK_X86_THREAD_H__

#include <types.h>

typedef struct
{
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t __esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;

} context_t;

typedef struct
{
  context_t ctx;
  uint32_t fake_ret;
  uint32_t entry;
  uint32_t id;
} start_context_t;

uint32_t		pok_context_create(uint32_t id,
					   uint32_t stack_size,
					   uint32_t entry);

void			pok_context_switch(uint32_t* old_sp,
					   uint32_t new_sp);

void			pok_context_reset(uint32_t stack_size,
					  uint32_t stack_addr);

#endif /* !__POK_X86_THREAD_H__ */

