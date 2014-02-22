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


#include <bsp.h>
#include <libc.h>
#include <errno.h>
#include <core/thread.h>

#include "thread.h"

#ifdef POK_NEEDS_THREADS

extern void pok_arch_thread_start(void);

uint32_t		pok_context_create (uint32_t id,
                                uint32_t stack_size,
                                uint32_t entry)
{
  context_t* sp;
  char*      stack_addr;

  stack_addr = pok_bsp_mem_alloc (stack_size);

  sp = (context_t *) (stack_addr + stack_size - sizeof (context_t));

  memset (sp, 0, sizeof (context_t));

  sp->r14     = entry;
  sp->r15     = id;
  sp->lr      = (uint32_t) pok_arch_thread_start;
  sp->sp      = (uint32_t) &sp->back_chain;

#ifdef POK_NEEDS_DEBUG
  printf ("ctxt_create %d: sp=%x\n", id, sp);
#endif

  return (uint32_t)sp;
}

uint32_t pok_context_reset(uint32_t stack_size,
			    uint32_t stack_addr)
{
  context_t* sp;
  uint32_t id;
  uint32_t entry;
  
  sp = (context_t *) (stack_addr + stack_size - 4 - sizeof (context_t));

  id = sp->r15;
  entry = sp->r14;
  memset (sp, 0, sizeof (context_t));

  sp->r14     = entry;
  sp->r15     = id;
  sp->lr      = (uint32_t) pok_arch_thread_start;
  sp->sp      = (uint32_t) &sp->back_chain;
  return 0;
}

#endif
