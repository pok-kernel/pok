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
 * \file    kernel/include/arch.h
 * \author  Julian Pidancet
 * \author  Julien Delange
 * \date    2008-2009
 * \brief   Generic interface to handle architectures
 */

#ifndef __POK_ARCH_H__
#define __POK_ARCH_H__

#include <types.h>
#include <errno.h>

/**
 * Function that initializes architecture concerns.
 */
pok_ret_t   pok_arch_init ();

/**
 * Disable interruptions
 */
pok_ret_t   pok_arch_preempt_disable ();

/**
 * Enable interruptions
 */
pok_ret_t   pok_arch_preempt_enable ();

/**
 * Function that do nothing. Useful for the idle task for example.
 */
pok_ret_t   pok_arch_idle ();

/**
 * Register an event (for example, an interruption)
 */
pok_ret_t   pok_arch_event_register (uint8_t vector, void (*handler)(void));

uint32_t	   pok_context_create (uint32_t thread_id,
                                uint32_t stack_size,
                                uint32_t entry);

void		   pok_context_switch (uint32_t* old_sp, uint32_t new_sp);

void			pok_context_reset(uint32_t stack_size,
					  uint32_t stack_addr);

pok_ret_t   pok_create_space (uint8_t partition_id, uint32_t addr, uint32_t size);

uint32_t	   pok_space_base_vaddr (uint32_t addr);

void        pok_dispatch_space (uint8_t partition_id,
				                    uint32_t user_pc,
                                uint32_t user_sp,
                                uint32_t kernel_sp,
                                uint32_t arg1,
                                uint32_t arg2);

/**
 * Create a new context in the given space
 */
uint32_t	   pok_space_context_create (uint8_t partition_id,
                                      uint32_t entry_rel,
                                      uint32_t stack_rel,
                                      uint32_t arg1,
                                      uint32_t arg2);


void pok_space_context_restart (uint32_t sp, uint32_t entry, uint32_t user_stack);

/**
 * Switch from one space to another
 */
pok_ret_t   pok_space_switch (uint8_t old_partition_id,
                              uint8_t new_partition_id);

/**
 * Returns the stack address for a the thread number N
 * in a partition.
 *
 * @arg partition_id indicates the partition that contains
 * the thread.
 *
 * @arg local_thread_id the thread-id of the thread
 * inside the partition.
 *
 * @return the stack address of the thread.
 */
uint32_t    pok_thread_stack_addr   (const uint8_t    partition_id,
                                     const uint32_t   local_thread_id);

#ifdef POK_ARCH_PPC
#include <arch/ppc/spinlock.h>
#endif

#ifdef POK_ARCH_X86
#include <arch/x86/spinlock.h>
#endif

#ifdef POK_ARCH_SPARC
#include <arch/sparc/spinlock.h>
#endif

#endif /* !__POK_ARCH_H__ */
