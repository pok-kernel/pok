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


#if defined POK_NEEDS_PORTS_QUEUEING || defined POK_NEEDS_PORTS_SAMPLING || defined POK_NEEDS_PORTS_VIRTUAL
extern uint32_t pok_ports_names_max_len;
#endif

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
#include <drivers/rtl8029.h>
#endif

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
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 args->arg2);

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
#ifdef POK_NEEDS_CONSOLE
      case POK_SYSCALL_GETCHAR:
         pok_check_ptr_or_return(infos->partition,
				 (void*)args->arg1,
				 NULL);

	 pok_cons_get_char((char *)args->arg1 + infos->base_addr);
	 return POK_ERRNO_OK;
#endif
     
#ifdef POK_NEEDS_PORTS_VIRTUAL
      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 pok_ports_names_max_len+1);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_id_t));

         return pok_port_virtual_id ( (char*) (args->arg1 + infos->base_addr),
                                      (pok_port_id_t*) (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_NB_DESTINATIONS:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(uint32_t));

         return pok_port_virtual_nb_destinations ( (pok_port_id_t) (args->arg1),
                                                   (uint32_t*) (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_DESTINATION:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg3,
                                 sizeof(uint32_t));

         return pok_port_virtual_destination ( (pok_port_id_t) (args->arg1),
                                               (uint32_t) (args->arg2),
                                               (uint32_t*) (args->arg3 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_GET_GLOBAL:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_id_t));

         return pok_port_virtual_get_global ((pok_port_id_t) (args->arg1),
                                             (pok_port_id_t*) (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_VIRTUAL_NODE:
	pok_check_ptr_or_return(infos->partition,
				(void*)args->arg2,
				sizeof(uint32_t));

	 return pok_port_virtual_node((uint32_t) (args->arg1), (uint8_t*) (args->arg2 + infos->base_addr));
#endif

#ifdef POK_NEEDS_MAC_ADDR
      case POK_SYSCALL_MIDDLEWARE_NODE_MAC_ADDR:
	pok_check_ptr_or_return(infos->partition,
				(void*)args->arg1,
				6);
	 
	pok_check_ptr_or_return(infos->partition,
				(void*)args->arg2,
				sizeof(uint32_t));

	 return pok_node_mac_addr((uint8_t*) (args->arg1 + infos->base_addr), (char*) (args->arg2 + infos->base_addr));
#endif
	   
#if defined POK_NEEDS_GETTICK
      case POK_SYSCALL_GETTICK:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint64_t));

         return pok_gettick_by_pointer ((uint64_t*) (args->arg1 + infos->base_addr));
         break;
#endif

      case POK_SYSCALL_THREAD_CREATE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint32_t));

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_thread_attr_t));

         return pok_partition_thread_create ((uint32_t*)          (args->arg1 + infos->base_addr),
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
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint32_t));

         return pok_sched_get_current ((uint32_t*) (args->arg1 + infos->base_addr));
         break;
#endif
   case POK_SYSCALL_THREAD_STATUS:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_thread_attr_t));

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
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint8_t));

         return pok_current_partition_get_id ((uint8_t*)(args->arg1 + infos->base_addr));
         break;

      case POK_SYSCALL_PARTITION_GET_PERIOD:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint64_t));

	 return pok_current_partition_get_period ((uint64_t*)(args->arg1 + infos->base_addr));
	 break;

      case POK_SYSCALL_PARTITION_GET_DURATION:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint64_t));

	 return pok_current_partition_get_duration ((uint64_t*)(args->arg1 + infos->base_addr));
	 break;

      case POK_SYSCALL_PARTITION_GET_LOCK_LEVEL:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(uint32_t));

	 return pok_current_partition_get_lock_level ((uint32_t*)(args->arg1 + infos->base_addr));
         break;

      case POK_SYSCALL_PARTITION_GET_OPERATING_MODE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(pok_partition_mode_t));

	 return pok_current_partition_get_operating_mode ((pok_partition_mode_t*)(args->arg1 + infos->base_addr));
         break;

      case POK_SYSCALL_PARTITION_GET_START_CONDITION:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(pok_start_condition_t));

	 return pok_current_partition_get_start_condition ((pok_start_condition_t*)(args->arg1 + infos->base_addr));
         break;
#endif

#ifdef POK_NEEDS_ERROR_HANDLING
      case POK_SYSCALL_ERROR_HANDLER_CREATE:
         return pok_error_thread_create (args->arg1 , (void*) (args->arg2));
         break;

      case POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 args->arg2);

         pok_error_raise_application_error ((char*) (args->arg1 + infos->base_addr), args->arg2);
         return POK_ERRNO_OK;
         break;

      case POK_SYSCALL_ERROR_GET:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(pok_error_status_t));

         return pok_error_get ((pok_error_status_t*) (args->arg1 + infos->base_addr));
         break;
#endif

         /* Middleware syscalls */
#ifdef POK_NEEDS_PORTS_SAMPLING
      case POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 pok_ports_names_max_len+1);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg5,
                                 sizeof(pok_port_id_t));

         return pok_port_sampling_create ((char*)(args->arg1 + infos->base_addr),
                                          (pok_port_size_t) args->arg2,
                                          (pok_port_direction_t) args->arg3,
                                          (uint64_t) args->arg4,
                                          (pok_port_id_t*) (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 args->arg3);

         return pok_port_sampling_write  ((const pok_port_id_t)args->arg1,
                                          (const void*) ((void*)args->arg2 + infos->base_addr),
                                          (const uint8_t) args->arg3);
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_READ:
         /* arg2 is checked later in pok_port_sampling_read() once we know access size */

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg3,
                                 sizeof(pok_port_size_t));

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg4,
                                 sizeof(bool_t));

         return pok_port_sampling_read ((const pok_port_id_t)args->arg1,
                                        (void*) args->arg2 + infos->base_addr,
                                        (pok_port_size_t*) (args->arg3 + infos->base_addr),
                                        (bool_t*) (args->arg4 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_SAMPLING_ID:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 pok_ports_names_max_len+1);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_id_t));

         return pok_port_sampling_id ((char*)(args->arg1 + infos->base_addr),
                                      (pok_port_id_t*)(args->arg2 + infos->base_addr));
         break;

#ifndef POK_GENERATED_CODE
      case POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_sampling_status_t));

         return pok_port_sampling_status ((const pok_port_id_t)args->arg1,
                                          (pok_port_sampling_status_t*) (args->arg2 + infos->base_addr));
         break;
#endif /* POK_GENERATED_CODE */
#endif /* POK_NEEDS_PORTS_SAMPLING */


#ifdef POK_NEEDS_PORTS_QUEUEING
      case POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 pok_ports_names_max_len+1);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg5,
                                 sizeof(pok_port_id_t));

         return pok_port_queueing_create ((char*)                          (args->arg1 + infos->base_addr),
                                          (pok_port_size_t)                 args->arg2,
                                          (pok_port_direction_t)            args->arg3,
                                          (pok_port_queueing_discipline_t)  args->arg4,
                                          (pok_port_id_t*)                 (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 args->arg3);

         return pok_port_queueing_send ((const pok_port_id_t)              args->arg1,
                                        (const void*)              ((void*)args->arg2 + infos->base_addr),
                                        (const uint8_t)                   (args->arg3),
                                        (const uint64_t)                   args->arg4);
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg4,
                                 args->arg3);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg5,
                                 sizeof(pok_port_size_t));

         return pok_port_queueing_receive ((const pok_port_id_t)         args->arg1,
                                           (uint64_t)                    args->arg2,
                                           (pok_port_size_t)             args->arg3,
                                           (void*)               ((void*)args->arg4 + infos->base_addr),
                                           (pok_port_size_t*)           (args->arg5 + infos->base_addr));
         break;

      case POK_SYSCALL_MIDDLEWARE_QUEUEING_ID:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 pok_ports_names_max_len+1);

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_id_t));

         return pok_port_queueing_id ((char*)           (args->arg1 + infos->base_addr),
                                      (pok_port_id_t*)  (args->arg2 + infos->base_addr));
         break;

#ifndef POK_GENERATED_CODE
      case POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_port_queueing_status_t));

         return pok_port_queueing_status ((const pok_port_id_t)           args->arg1,
                                          (pok_port_queueing_status_t*)  (args->arg2 + infos->base_addr));
         break;
#endif
#endif /* POK_NEEDS_PORTS_QUEUEING */

#ifdef POK_NEEDS_LOCKOBJECTS
      case POK_SYSCALL_LOCKOBJ_CREATE:
         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg1,
                                 sizeof(pok_lockobj_id_t));

         pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(pok_lockobj_attr_t));

         return pok_lockobj_partition_create ((pok_lockobj_id_t*)    (args->arg1 + infos->base_addr),
                                              (pok_lockobj_attr_t*)  (args->arg2 + infos->base_addr));
         break;

      case POK_SYSCALL_LOCKOBJ_OPERATION:
         if (args->arg2 == NULL)
         {
            return pok_lockobj_partition_wrapper ((const uint8_t) args->arg1, NULL);
         }
         else
         {
            pok_check_ptr_or_return(infos->partition,
                                    (void*)args->arg2,
                                    sizeof(pok_lockobj_attr_t));

            return pok_lockobj_partition_wrapper ((const uint8_t)            args->arg1,
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
        pok_check_ptr_or_return(infos->partition,
                                (void*)args->arg1,
                                sizeof(s_pci_device));

       return pci_register((void*)args->arg1 + infos->base_addr, infos->partition);
       break;
#endif /* POK_NEEDS_PCI */

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
   case POK_SYSCALL_RTL8929_READ:
     pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(uint32_t));

     rtl8029_read ( (pok_port_id_t) (args->arg1),
		    (uint32_t*) (args->arg2 + infos->base_addr),
		    (uint32_t) (args->arg3));
     
     break;

   case POK_SYSCALL_RTL8929_POLL_AND_READ:
     pok_check_ptr_or_return(infos->partition,
                                 (void*)args->arg2,
                                 sizeof(uint32_t));

     rtl8029_poll_and_read ( (pok_port_id_t) (args->arg1),
			     (uint32_t*) (args->arg2 + infos->base_addr),
			     (uint32_t) (args->arg3));
     
     break;
     
   case POK_SYSCALL_RTL8929_WRITE:
     pok_check_ptr_or_return(infos->partition,
			     (void*)args->arg2,
			     sizeof(uint32_t));

     rtl8029_write ( (pok_port_id_t) (args->arg1),
			    (uint32_t*) (args->arg2 + infos->base_addr),
			    (uint32_t) (args->arg3));
     break;
     
   case POK_SYSCALL_RTL8929_POLL:
     rtl8029_poll();
     break;
     
   case POK_SYSCALL_RTL8929_CLEAR:
     rtl8029_clear();
     break;
     
   case POK_SYSCALL_RTL8929_INIT:
     rtl8029_init();
     break;
#endif

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
