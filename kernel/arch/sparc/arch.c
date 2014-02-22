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
 */

#include <types.h>
#include <errno.h>
#include <core/partition.h>
#include "traps.h"
#include "space.h"
#include "psr.h"
#include "sparc_conf.h"
#include "syscalls.h"

/**
 * Initialize all SPARC managers (traps, syscalls, space).
 */
pok_ret_t pok_arch_init ()
{
  traps_init();
  psr_disable_interupt();
  psr_enable_traps();

  pok_arch_space_init();
  pok_syscalls_init();

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_disable()
{
  psr_disable_interupt();

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_enable()
{
  psr_enable_interupt();

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_idle()
{
  while (1)
  {
    /* Leon3 Only ? */
    asm volatile ("wr %g0, %asr19");
  }

   return (POK_ERRNO_OK);
}

/**
 * Attach the handler to the given trap number (vector).
 * @see pok_sparc_isr
 */
pok_ret_t pok_arch_event_register (uint8_t vector, void (*handler)(void))
{
  if (pok_sparc_isr[vector] == NULL)
  {
    pok_sparc_isr[vector] = handler;
    return (POK_ERRNO_OK);
  }
  else
  {
    return (POK_ERRNO_UNAVAILABLE);
  }
}

/**
 * Compute the stack adress for the given thread.
 */
uint32_t    pok_thread_stack_addr   (const uint8_t    partition_id,
                                     const uint32_t   local_thread_id)
{
  return pok_partitions[partition_id].size - (local_thread_id * POK_USER_STACK_SIZE);
}


