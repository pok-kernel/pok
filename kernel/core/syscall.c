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
 * Created by julien on Wed Oct 21 13:12:27 2009
 */

#include <bsp.h>
#include <types.h>
#include <libc.h>
#include <arch/x86/ioports.h>
#include <arch/x86/pci.h>

#include <errno.h>
#include <core/debug.h>
#include <core/syscall.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/lockobj.h>
#include <core/time.h>
#include <core/error.h>

#include <middleware/port.h>

/**
 * \file kernel/core/syscalls.c
 * \brief This file implement generic system calls
 * \author Julien Delange
 */

pok_ret_t pok_core_syscall (const pok_syscall_id_t       syscall_id,
                            const pok_syscall_args_t*    args,
                            const pok_syscall_info_t*    infos)
{
   switch (syscall_id)
   {
#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG)
      case POK_SYSCALL_CONSWRITE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         if (pok_cons_write ((const char*)args->arg1 + infos->base_addr, args->arg2))
         {
            return POK_ERRNO_OK;
         }
         else
         {
            return POK_ERRNO_EINVAL;
         }
         break;
#endif

#ifdef POK_NEEDS_PORTS_VIRTUAL
      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_virtual_id ( (char*) (args->arg1 + infos->base_addr), (pok_port_id_t*) (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_NB_DESTINATIONS:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_virtual_nb_destinations ( (pok_port_id_t) (args->arg1), (uint32_t*) (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_DESTINATION:
         POK_CHECK_PTR_OR_RETURN(infos->partition, ((void*) args->arg3)+infos->base_addr)
         return pok_port_virtual_destination ( (pok_port_id_t) (args->arg1), (uint32_t) (args->arg2), (uint32_t*) (args->arg3 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_GET_GLOBAL:
         POK_CHECK_PTR_OR_RETURN(infos->partition, (void*) (args->arg2 + infos->base_addr))
         return pok_port_virtual_get_global ((pok_port_id_t) (args->arg1), (pok_port_id_t*) (args->arg2 + infos->base_addr));
         break;

#endif

#if 1//defined POK_NEEDS_GETTICK
      case POK_SYSCALL_GETTICK:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         return pok_gettick_by_pointer ((uint64_t*) (args->arg1 + infos->base_addr));
         break;
#endif

      case POK_SYSCALL_THREAD_CREATE:
         return pok_partition_thread_create  ((uint32_t*)         (args->arg1 + infos->base_addr),
                                             (pok_thread_attr_t*) (args->arg2 + infos->base_addr),
                                             (uint8_t)            infos->partition);
         break;

#ifdef POK_NEEDS_THREAD_SLEEP
      case POK_SYSCALL_THREAD_SLEEP:
         return pok_thread_sleep (args->arg1);
         break;
#endif

#ifdef POK_NEEDS_THREAD_SLEEP_UNTIL
      case POK_SYSCALL_THREAD_SLEEP_UNTIL:
         return pok_thread_sleep_until (args->arg1);
         break;
#endif

      case POK_SYSCALL_THREAD_PERIOD:
         return pok_sched_end_period ();
         break;

#if defined (POK_NEEDS_THREAD_SUSPEND) || defined (POK_NEEDS_ERROR_HANDLING)
      case POK_SYSCALL_THREAD_SUSPEND:
         return pok_thread_suspend ();
         break;
#endif

#ifdef POK_NEEDS_THREAD_ID
      case POK_SYSCALL_THREAD_ID:
         return pok_sched_get_current ((uint32_t*) (args->arg1 + infos->base_addr));
         break;
#endif
   case POK_SYSCALL_THREAD_STATUS:
	      return pok_thread_get_status (args->arg1, (pok_thread_attr_t*) (args->arg2 + infos->base_addr));
         break;

   case POK_SYSCALL_THREAD_DELAYED_START:
     return pok_thread_delayed_start (args->arg1, args->arg2);
     break;
   case POK_SYSCALL_THREAD_SET_PRIORITY:
	   return pok_thread_set_priority (args->arg1, args->arg2);
	   break;

   case POK_SYSCALL_THREAD_RESUME:
	   return pok_thread_resume (args->arg1);
	   break;
   case POK_SYSCALL_THREAD_SUSPEND_TARGET:
	   return pok_thread_suspend_target (args->arg1);
	   break;

#ifdef POK_NEEDS_ERROR_HANDLING

/**
 * We consider that we don't need the THREAD_RESTART
 * syscall if we don't handle errors
 */
      case POK_SYSCALL_THREAD_RESTART:
         return pok_partition_restart_thread (args->arg1);
         break;

      case POK_SYSCALL_THREAD_STOP:
         return pok_partition_stop_thread (args->arg1);
         break;

      /**
       * STOPSELF used by the error thread
       */
      case POK_SYSCALL_THREAD_STOPSELF:
         pok_sched_stop_self ();
         return POK_ERRNO_OK;
         break;

#endif

#ifdef POK_NEEDS_PARTITIONS
      case POK_SYSCALL_PARTITION_SET_MODE:
         return pok_partition_set_mode_current ((pok_partition_mode_t)args->arg1);
         break;
      case POK_SYSCALL_PARTITION_GET_ID:
	return pok_current_partition_get_id ((uint8_t*)(args->arg1 + infos->base_addr));
         break;
      case POK_SYSCALL_PARTITION_GET_PERIOD:
	return pok_current_partition_get_period ((uint64_t*)(args->arg1 + infos->base_addr));
	 break;
      case POK_SYSCALL_PARTITION_GET_DURATION:
	return pok_current_partition_get_duration ((uint64_t*)(args->arg1 + infos->base_addr));
	 break;
      case POK_SYSCALL_PARTITION_GET_LOCK_LEVEL:
	return pok_current_partition_get_lock_level ((uint32_t*)(args->arg1 + infos->base_addr));
         break;
      case POK_SYSCALL_PARTITION_GET_OPERATING_MODE:
	return pok_current_partition_get_operating_mode ((pok_partition_mode_t*)(args->arg1 + infos->base_addr));
         break;
      case POK_SYSCALL_PARTITION_GET_START_CONDITION:
	return pok_current_partition_get_start_condition ((pok_start_condition_t*)(args->arg1 + infos->base_addr));
         break;
#endif

#ifdef POK_NEEDS_ERROR_HANDLING
      case POK_SYSCALL_ERROR_HANDLER_CREATE:
         return pok_error_thread_create (args->arg1 , (void*) (args->arg2));
         break;

      case POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         pok_error_raise_application_error ((char*) (args->arg1 + infos->base_addr), args->arg2);
         return POK_ERRNO_OK;
         break;

      case POK_SYSCALL_ERROR_GET:
         return pok_error_get ((pok_error_status_t*) (args->arg1 + infos->base_addr));
         break;
#endif

         /* Middleware syscalls */
#ifdef POK_NEEDS_PORTS_SAMPLING
      case POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE: 
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg5 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         return pok_port_sampling_create  ((char*)(args->arg1 + infos->base_addr),
                                          (pok_port_size_t) args->arg2,
                                          (pok_port_direction_t) args->arg3,
                                          (uint64_t) args->arg4,
                                          (pok_port_id_t*) (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)

         return pok_port_sampling_write   ((const pok_port_id_t)args->arg1,
                                          (const void*) ((void*)args->arg2 + infos->base_addr),
                                          (const uint8_t) args->arg3);
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_READ:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg4 + infos->base_addr)
         return pok_port_sampling_read ((const pok_port_id_t)args->arg1,
                                       (void*) args->arg2 + infos->base_addr,
                                       (pok_port_size_t*) (args->arg3 + infos->base_addr),
                                       (bool_t*) (args->arg4 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_ID:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_sampling_id   ((char*)(args->arg1 + infos->base_addr),
                                       (pok_port_id_t*)(args->arg2 + infos->base_addr));
         break;
          
#ifndef POK_GENERATED_CODE
      case POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2+infos->base_addr)
         return pok_port_sampling_status ((const pok_port_id_t)args->arg1,
                                          (pok_port_sampling_status_t*) (args->arg2 + infos->base_addr));
         break;
#endif /* POK_GENERATED_CODE */
#endif /* POK_NEEDS_PORTS_SAMPLING */


#ifdef POK_NEEDS_PORTS_QUEUEING
      case POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE: 
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg5 + infos->base_addr)
         return pok_port_queueing_create ((char*)                          (args->arg1 + infos->base_addr), 
                                         (pok_port_size_t)                 args->arg2, 
                                         (pok_port_direction_t)            args->arg3, 
                                         (pok_port_queueing_discipline_t)  args->arg4, 
                                         (pok_port_id_t*)                  (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_queueing_send ((const pok_port_id_t)              args->arg1,
                                       (const void*)                       ((void*)args->arg2 + infos->base_addr), 
                                       (const uint8_t)                     (args->arg3),
                                       (const uint64_t)                    args->arg4);
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg4 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg5 + infos->base_addr)
         return pok_port_queueing_receive ((const pok_port_id_t)   args->arg1, 
                                          (uint64_t)              args->arg2,
                                          (pok_port_size_t)       args->arg3,
                                          (void*)                 ((void*)args->arg4 + infos->base_addr), 
                                          (pok_port_size_t*)      (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_ID:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_queueing_id ((char*)          (args->arg1 + infos->base_addr),
                                    (pok_port_id_t*)  (args->arg2 + infos->base_addr));
         break;

#ifndef POK_GENERATED_CODE
      case POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
         return pok_port_queueing_status     ((const pok_port_id_t)           args->arg1,
                                             (pok_port_queueing_status_t*)    (args->arg2 + infos->base_addr));
         break;
#endif
#endif /* POK_NEEDS_PORTS_QUEUEING */

#ifdef POK_NEEDS_LOCKOBJECTS
      case POK_SYSCALL_LOCKOBJ_CREATE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2+infos->base_addr)
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1+infos->base_addr)
         return pok_lockobj_partition_create    ((pok_lockobj_id_t*)    (args->arg1 + infos->base_addr),
                                                (pok_lockobj_attr_t*)   (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_LOCKOBJ_OPERATION:
         if (args->arg2 == NULL)
         {
            return pok_lockobj_partition_wrapper ((const uint8_t) args->arg1, NULL);
         }
         else
         {
            POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
            return pok_lockobj_partition_wrapper   ((const uint8_t) args->arg1,
                                                   (pok_lockobj_lockattr_t*) (args->arg2 + infos->base_addr));
         }
         break;
#endif /* POK_NEEDS_LOCKOBJECTS */

#ifdef POK_NEEDS_IO
      case POK_SYSCALL_INB:
         if ((args->arg1 < pok_partitions[infos->partition].io_min) ||
             (args->arg1 > pok_partitions[infos->partition].io_max))
         {
            return -POK_ERRNO_EPERM;
         }
         else
         {
            return inb((unsigned short) args->arg1);
         }
         break;

      case POK_SYSCALL_OUTB:
         if ((args->arg1 < pok_partitions[infos->partition].io_min) ||
             (args->arg1 > pok_partitions[infos->partition].io_max))
         {
            return -POK_ERRNO_EPERM;
         }
         else
         {
            outb((unsigned short) args->arg1, (unsigned char) args->arg2);
            return POK_ERRNO_OK;
         }
       break;
#endif /* POK_NEEDS_IO */

#ifdef POK_NEEDS_PCI
     case POK_SYSCALL_PCI_REGISTER:
       POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
       return pci_register((void*)args->arg1 + infos->base_addr, infos->partition);
       break;
#endif /* POK_NEEDS_PCI */

      /**
       * This shall register an irq handler with the meta_handler for the
       * requested interrupt number.
       * arg1: vector nubmer, 
       * arg2: handler/callback function
       */
     case POK_SYSCALL_IRQ_REGISTER_HANDLER:
//       POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
       POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
       /* arg1 is just a number, arg2 is a pointer to a function in the
	* partition, therefore the partitions base_addr must be added to become
	* a valid pointer for the kernel.
	*/
       return pok_bsp_irq_register_hw (args->arg1, 
		 (void(*)(unsigned, void*)) ((args->arg2 + infos->base_addr)) );
       break;

      /**
       * Here is the default syscall handler. In this case, the syscall
       * ID was not properly identified and thus, we should return an 
       * error. If error management is activated, we raise an error
       * in kernel of partitions, calling the error handler.
       */
      default:
#ifdef POK_NEEDS_ERROR_HANDLING
         pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
         pok_sched_activate_error_thread ();
#else
         #ifdef POK_NEEDS_DEBUG
            printf ("Tried to use syscall %d\n", syscall_id);
         #endif
         POK_FATAL ("Unknown syscall");
#endif
         break;
   }

   return POK_ERRNO_EINVAL;
}
