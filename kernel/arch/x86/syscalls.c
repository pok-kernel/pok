/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

/**
 * \file kernel/arch/x86/syscalls.c
 * \brief This file implement system-calls for x86 platform
 * \author Julian Pidancet
 * \author Julien Delange
 * \author Laurent Lec
 */

#include <core/debug.h>
#include <core/partition.h>
#include <core/syscall.h>
#include <errno.h>

#include "event.h"
#include "gdt.h"

#define PARTITION_ID(cs) (((cs >> 3) - 3 - POK_CONFIG_NB_PROCESSORS) / 2)

INTERRUPT_HANDLER_syscall(syscall_gate) {
  pok_syscall_info_t syscall_info;
  pok_ret_t syscall_ret;
  pok_syscall_args_t *syscall_args;
  pok_syscall_id_t syscall_id;

  /*
   * Give informations about syscalls: which partition, thread
   * initiates the syscall, the base addr of the partition and so on.
   */
  syscall_info.partition = PARTITION_ID(frame->cs);
  syscall_info.base_addr = pok_partitions[syscall_info.partition].base_addr;
  syscall_info.thread = POK_SCHED_CURRENT_THREAD;

  syscall_args = (pok_syscall_args_t *)(frame->ebx + syscall_info.base_addr);

  /*
   * Get the syscall id in the eax register
   */
  syscall_id = (pok_syscall_id_t)frame->eax;

  /*
   * Check that pointer is inside the adress space
   */
  if (pok_check_ptr_in_partition(syscall_info.partition, (void *)frame->ebx,
                                 sizeof(pok_syscall_args_t)) == 0) {
    syscall_ret = POK_ERRNO_EINVAL;
  } else {
    /*
     * Perform the syscall baby !
     */
    syscall_ret = pok_core_syscall(syscall_id, syscall_args, &syscall_info);
  }

  /*
   * And finally, put the return value in eax register
   */
  asm("movl %0, %%eax  \n" : : "m"(syscall_ret));
}

/**
 * Init system calls
 */
pok_ret_t pok_syscall_init() {
  pok_idt_set_gate(POK_SYSCALL_INT_NUMBER, GDT_CORE_CODE_SEGMENT << 3,
                   (uint32_t)syscall_gate, IDTE_INTERRUPT, 3);

  return (POK_ERRNO_OK);
}
