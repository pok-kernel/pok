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
 * @brief  Thread management.
 */

#include <bsp.h>
#include <libc.h>
#include <errno.h>
#include <core/thread.h>
#include "thread.h"
#include "context_offset.h"
#include "ioports.h"

#ifdef POK_NEEDS_THREADS

extern char _idlestack;

/**
 * Funcion only used to initialize the idle thread.
 */
uint32_t pok_context_create (uint32_t id,
                             uint32_t stack_size,
                             uint32_t entry)
{
  (void)stack_size;
  char *ctx = (char *)(&_idlestack - 0x40);

  *(uint32_t *)(ctx - RESTORE_CNT_OFFSET) = 1;
  *(uint32_t *)(ctx - PC_OFFSET) = entry;
  *(uint32_t *)(ctx - NPC_OFFSET) = entry + 4;
  *(uint32_t *)(ctx - I1_OFFSET) = id;

#ifdef POK_NEEDS_DEBUG
  printf ("ctxt_create %d: sp=%x\n", id, ctx);
#endif

  return (uint32_t)ctx;
}

/**
 * Global variable for context switch in SPARC.
 * @see pok_context_switch()
 */
uint32_t pok_arch_sp;

/**
 * Save interrupted stack pointer and set
 * stack pointer for the next context.
 * @see pok_arch_sp
 */
void pok_context_switch (uint32_t* old_sp,
                         uint32_t new_sp)
{
  *old_sp = pok_arch_sp;
  pok_arch_sp = new_sp;
}

void                    pok_context_reset(uint32_t stack_size,
                                          uint32_t stack_addr)
{
  (void)stack_size;
  (void)stack_addr;
  uint32_t id;
  uint32_t entry;

  char *ctx = (char *)(&_idlestack - 0x40);

  id = *(uint32_t *)(ctx - PC_OFFSET);
  entry =  *(uint32_t *)(ctx - I1_OFFSET);

  *(uint32_t *)(ctx - RESTORE_CNT_OFFSET) = 1;
  *(uint32_t *)(ctx - PC_OFFSET) = entry;
  *(uint32_t *)(ctx - NPC_OFFSET) = entry + 4;
  *(uint32_t *)(ctx - I1_OFFSET) = id;
}

#endif
