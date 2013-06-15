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


#include <errno.h>
#include <core/debug.h>
#include <core/syscall.h>
#include <core/partition.h>

#include <types.h>
#include <libc.h>

void pok_arch_sc_int(uint32_t num, uint32_t arg1, uint32_t arg2,
                     uint32_t arg3, uint32_t arg4, uint32_t arg5)
{
   uint8_t              part_id;

   pok_syscall_info_t   syscall_info;
   pok_syscall_args_t   syscall_args;
   pok_syscall_id_t     syscall_id;

   part_id  = pok_current_partition;

   /* prepare syscall_info */
   syscall_info.partition = part_id;
   syscall_info.base_addr = pok_partitions[part_id].base_addr;
   syscall_info.thread    = POK_SCHED_CURRENT_THREAD;

   /* prepare syscall_args */
   syscall_args.arg1 = arg1;
   syscall_args.arg2 = arg2;
   syscall_args.arg3 = arg3;
   syscall_args.arg4 = arg4;
   syscall_args.arg5 = arg5;

   syscall_args.nargs = 5;

   /* prepare syscall_id */
   syscall_id = (pok_syscall_id_t) num;

   if (POK_CHECK_PTR_IN_PARTITION(syscall_info.partition, &syscall_args) != 0)
   {
      /*
       * Perform the syscall baby !
       */
     pok_core_syscall (syscall_id, &syscall_args, &syscall_info);
   }
}
