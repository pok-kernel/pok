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
 * \file    arch/ppc/arch.c
 * \author  Tristan Gingold
 * \date    2009
 * \brief   Provide generic architecture access for PPC architecture
 */

#include <types.h>
#include <errno.h>
#include <core/partition.h>
#include "msr.h"

extern void pok_arch_space_init (void);

static inline unsigned int get_msr (void)
{
  unsigned int res;
  asm ("mfmsr %0\n" : "=r" (res));
  return res;
}

static inline void set_msr (unsigned int val)
{
  asm volatile ("mtmsr %0\n" : : "r" (val));
}

pok_ret_t pok_arch_init ()
{
  set_msr (MSR_IP);
#if POK_NEEDS_PARTITIONS
  pok_arch_space_init();
#endif

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_disable()
{
  unsigned int msr;

  msr = get_msr();
  msr &= ~MSR_EE;
  set_msr(msr);
  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_preempt_enable()
{
  unsigned int msr;

  msr = get_msr();
  msr |= MSR_EE;
  set_msr(msr);

  return (POK_ERRNO_OK);
}

pok_ret_t pok_arch_idle()
{
   while (1)
   {
   }

   return (POK_ERRNO_OK);	
}

pok_ret_t pok_arch_event_register (uint8_t vector, void (*handler)(void))
{
  (void) vector;
  (void) handler;

  return (POK_ERRNO_OK);
}


uint32_t    pok_thread_stack_addr   (const uint8_t    partition_id,
                                     const uint32_t   local_thread_id)
{
   return pok_partitions[partition_id].size - 16 - (local_thread_id * POK_USER_STACK_SIZE);
}


