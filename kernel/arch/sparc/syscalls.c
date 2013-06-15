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
 * @brief  Syscalls management in SPARC.
 */

#include <errno.h>
#include <core/debug.h>
#include <core/syscall.h>
#include <core/partition.h>

#include <types.h>
#include <libc.h>

#include "thread.h"
#include "context_offset.h"
#include "traps.h"
#include "arch.h"

/**
 * Syscalls handler.
 */
void pok_arch_sc_int(void)
{
  uint8_t *ctx = (uint8_t *)pok_arch_sp;
  uint32_t num = *(uint32_t *)((char *)ctx - I0_OFFSET);
  uint8_t              part_id;
  pok_syscall_info_t   syscall_info;
  pok_ret_t            syscall_ret;
  pok_syscall_args_t   syscall_args;
  pok_syscall_id_t     syscall_id;


  part_id  = pok_current_partition;

  /* prepare syscall_info */
  syscall_info.partition = part_id;
  syscall_info.base_addr = pok_partitions[part_id].base_addr;
  syscall_info.thread    = POK_SCHED_CURRENT_THREAD;

  /* prepare syscall_args */
  syscall_args.arg1 = *(uint32_t *)(ctx - I1_OFFSET);
  syscall_args.arg2 = *(uint32_t *)(ctx - I2_OFFSET);
  syscall_args.arg3 = *(uint32_t *)(ctx - I3_OFFSET);
  syscall_args.arg4 = *(uint32_t *)(ctx - I4_OFFSET);
  syscall_args.arg5 = *(uint32_t *)(ctx - I5_OFFSET);

  syscall_args.nargs = 5;

  /* prepare syscall_id */
  syscall_id = (pok_syscall_id_t) num;

  /*
   * No pointer check needed, syscall_args is allocated in kernel stack.
   */
  syscall_ret = pok_core_syscall (syscall_id, &syscall_args, &syscall_info);

  *(uint32_t *)(ctx - I0_OFFSET) = syscall_ret;
  *(uint32_t *)(ctx - PC_OFFSET) += 4; // skip "ta" instruction
  *(uint32_t *)(ctx - NPC_OFFSET) += 4;
}

/**
 * Syscalls initialization.
 * Just register the syscall handler.
 */
void pok_syscalls_init(void)
{
  pok_arch_event_register(SPARC_TRAP_SYSCALL_BASE + 0x2, pok_arch_sc_int);
}
