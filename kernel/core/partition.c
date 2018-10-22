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
 * \file partition.c
 * \brief This file provides functions for partitioning services
 * \author Julien Delange
 *
 * The definition of useful structures can be found in partition.h
 * header file. To enable partitioning services, you must set the
 * POK_NEEDS_PARTITIONS maccro.
 */

#ifdef POK_NEEDS_PARTITIONS

#include <arch.h>
#include <bsp.h>
#include <errno.h>
#include <dependencies.h>
#include <core/sched.h>
#include <core/error.h>
#include <core/debug.h>
#include <core/thread.h>
#include <core/loader.h>
#include <core/partition.h>
#include <core/instrumentation.h>
#include <core/time.h>

#include <libc.h>

/**
 * \brief The array that contains ALL partitions in the system.
 */
pok_partition_t pok_partitions[POK_CONFIG_NB_PARTITIONS];


uint8_t			 pok_partitions_index = 0;

extern uint64_t		pok_sched_slots[];


/**
 **\brief Setup the scheduler used in partition pid
 */
void pok_partition_setup_scheduler (const uint8_t pid)
{
#ifdef POK_CONFIG_PARTITIONS_SCHEDULER
      switch (((pok_sched_t[])POK_CONFIG_PARTITIONS_SCHEDULER)[pid])
      {
#ifdef POK_NEEDS_SCHED_RMS
         case POK_SCHED_RMS:
            pok_partitions[pid].sched_func  = &pok_sched_part_rms;
            break;
#endif

            /*
             * Default scheduling algorithm is Round Robin.
             * Yes, it sucks
             */
         default:
            pok_partitions[pid].sched_func  = &pok_sched_part_rr;
            break;
      }
#else
      pok_partitions[pid].sched_func  = &pok_sched_part_rr;
#endif
}

/**
 * \brief Reinitialize a partition from scratch
 *
 * This service is only used when we have to retrieve
 * and handle errors.
 */

#ifdef POK_NEEDS_ERROR_HANDLING
void pok_partition_reinit (const uint8_t pid)
{
   uint32_t tmp;
   /*
    * FIXME: reset queueing/sampling ports too
    */
   pok_partition_setup_scheduler (pid);

   pok_partitions[pid].thread_index = 0;
   pok_partitions[pid].current_thread = pok_partitions[pid].thread_index_low;
   pok_partitions[pid].prev_thread =  IDLE_THREAD; // breaks the rule of prev_thread not being idle, but it's just for init

#ifdef POK_NEEDS_ERROR_HANDLING
   pok_partitions[pid].thread_error = 0;
   pok_partitions[pid].error_status.failed_thread = 0;
   pok_partitions[pid].error_status.failed_addr   = 0;
   pok_partitions[pid].error_status.error_kind    = POK_ERROR_KIND_INVALID;
   pok_partitions[pid].error_status.msg_size      = 0;
#endif

   pok_loader_load_partition (pid, pok_partitions[pid].base_addr - pok_partitions[pid].base_vaddr, &tmp);

   pok_partitions[pid].thread_main_entry = tmp;

   pok_partition_setup_main_thread (pid);
}
#endif

/**
 * Setup the main thread of partition with number \a pid
 */
void pok_partition_setup_main_thread (const uint8_t pid)
{
   uint32_t main_thread;
   pok_thread_attr_t attr;

   attr.entry = (uint32_t*)pok_partitions[pid].thread_main_entry;
   attr.priority = 1;
   attr.deadline = 0;
   attr.period   = 0;
   attr.time_capacity = 0;

   pok_partition_thread_create (&main_thread, &attr, pid);
   pok_partitions[pid].thread_main = main_thread;
}

/**
 * \brief Initialize all partitions.
 *
 * It initializes everything, load the program, set thread
 * and lockobjects bounds.
 */
pok_ret_t pok_partition_init ()
{
   uint8_t     i;
   uint32_t    threads_index = 0;

   const uint32_t	partition_size[POK_CONFIG_NB_PARTITIONS] = POK_CONFIG_PARTITIONS_SIZE;
#ifdef POK_CONFIG_PARTITIONS_LOADADDR
   const uint32_t	program_loadaddr[POK_CONFIG_NB_PARTITIONS]
      = POK_CONFIG_PROGRAM_LOADADDR;
#endif
#ifdef POK_NEEDS_LOCKOBJECTS
   uint8_t lockobj_index = 0;
#endif

   for (i = 0 ; i < POK_CONFIG_NB_PARTITIONS ; i++)
   {
      uint32_t size = partition_size[i];
#ifndef POK_CONFIG_PARTITIONS_LOADADDR
      uint32_t base_addr = (uint32_t)pok_bsp_mem_alloc(partition_size[i]);
#else
      uint32_t base_addr = program_loadaddr[i];
#endif
      uint32_t program_entry;
      uint32_t base_vaddr = pok_space_base_vaddr(base_addr);

      pok_partitions[i].base_addr   = base_addr;
      pok_partitions[i].size        = size;
      pok_partitions[i].sched       = POK_SCHED_RR;
     
#ifdef POK_NEEDS_COVERAGE_INFOS
#include <libc.h>
      printf ("[XCOV] Partition %d loaded at addr virt=|%x|, phys=|%x|\n", i, base_vaddr, base_addr);
#endif

      pok_partition_setup_scheduler (i);

      pok_create_space (i, base_addr, size);

      pok_partitions[i].base_vaddr = base_vaddr;
      /* Set the memory space and so on */
      
      pok_partitions[i].thread_index_low  = threads_index;
      pok_partitions[i].nthreads          = ((uint32_t[]) POK_CONFIG_PARTITIONS_NTHREADS) [i];

#ifdef POK_NEEDS_ERROR_HANDLING
      if (pok_partitions[i].nthreads <= 1)
      {
         pok_partition_error (i, POK_ERROR_KIND_PARTITION_CONFIGURATION);
      }
#endif

#ifdef POK_CONFIG_PARTITIONS_SCHEDULER
      pok_partitions[i].sched             = ((pok_sched_t[]) POK_CONFIG_PARTITIONS_SCHEDULER) [i];
#endif

      pok_partitions[i].thread_index_high = pok_partitions[i].thread_index_low + ((uint32_t[]) POK_CONFIG_PARTITIONS_NTHREADS) [i];
      pok_partitions[i].activation        = 0;
      pok_partitions[i].period            = 0;
      pok_partitions[i].thread_index      = 0;
      pok_partitions[i].thread_main       = 0;
      pok_partitions[i].current_thread    = IDLE_THREAD;
      pok_partitions[i].prev_thread       = IDLE_THREAD; // breaks the rule of prev_thread not being idle, but it's just for init

#ifdef POK_NEEDS_SCHED_HFPPS
      pok_partitions[i].payback = 0;
#endif /* POK_NEEDS_SCHED_HFPPS */

      threads_index                       = threads_index + pok_partitions[i].nthreads;
      /* Initialize the threading stuff */

      pok_partitions[i].mode              = POK_PARTITION_MODE_INIT_WARM;

#ifdef POK_NEEDS_LOCKOBJECTS
      pok_partitions[i].lockobj_index_low    = lockobj_index;
      pok_partitions[i].lockobj_index_high   = lockobj_index + ((uint8_t[]) POK_CONFIG_PARTITIONS_NLOCKOBJECTS[i]);
      pok_partitions[i].nlockobjs            = ((uint8_t[]) POK_CONFIG_PARTITIONS_NLOCKOBJECTS[i]);
      lockobj_index                          = lockobj_index + pok_partitions[i].nlockobjs;
      /* Initialize mutexes stuff */
#endif

#ifdef POK_NEEDS_ERROR_HANDLING
      pok_partitions[i].thread_error      = 0;
      pok_partitions[i].error_status.failed_thread = 0;
      pok_partitions[i].error_status.failed_addr   = 0;
      pok_partitions[i].error_status.error_kind    = POK_ERROR_KIND_INVALID;
      pok_partitions[i].error_status.msg_size      = 0;
#endif

      pok_loader_load_partition (i, base_addr - base_vaddr, &program_entry);
      /*
       * Load the partition in its address space
       */
      pok_partitions[i].thread_main_entry = program_entry;
      
      pok_partitions[i].lock_level = 0;
      pok_partitions[i].start_condition = NORMAL_START;

#ifdef POK_NEEDS_INSTRUMENTATION
      pok_instrumentation_partition_archi (i);
#endif

      pok_partition_setup_main_thread (i);
      pok_partitions[i].current_thread    = pok_partitions[i].thread_main;
   }

   return POK_ERRNO_OK;
}

/**
 * Change the current mode of the partition. Possible mode
 * are describe in core/partition.h. Returns
 * POK_ERRNO_PARTITION_MODE when requested mode is invalid.
 * Else, returns POK_ERRNO_OK
 */
pok_ret_t pok_partition_set_mode (const uint8_t pid, const pok_partition_mode_t mode)
{
   switch (mode)
   {
      case POK_PARTITION_MODE_NORMAL:
         /*
          * We first check that a partition that wants to go
          * to the NORMAL mode is currently in the INIT mode
          */

         if (pok_partitions[pid].mode == POK_PARTITION_MODE_IDLE)
         {
            return POK_ERRNO_PARTITION_MODE;
         }

         if (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_main)
         {
            return POK_ERRNO_PARTITION_MODE;
         }

         pok_partitions[pid].mode = mode;  /* Here, we change the mode */

	 pok_thread_t* thread;
	 unsigned int i;
	 for (i = 0; i < pok_partitions[pid].nthreads; i++)
	 {
		 thread = &(pok_threads[POK_CURRENT_PARTITION.thread_index_low + i]);
		 if ((long long)thread->period == -1) {//-1 <==> ARINC INFINITE_TIME_VALUE
			 if(thread->state == POK_STATE_DELAYED_START) { // delayed start, the delay is in the wakeup time
				 if(!thread->wakeup_time) {
					 thread->state = POK_STATE_RUNNABLE;
				 } else {
					 thread->state = POK_STATE_WAITING;
				 }
				 thread->wakeup_time += POK_GETTICK();
				 if(thread->time_capacity>0)
				   thread->end_time =  thread->wakeup_time + thread->time_capacity;
			 }
		 } else {
			 if(thread->state == POK_STATE_DELAYED_START) { // delayed start, the delay is in the wakeup time
			   if(!thread->wakeup_time) {
			     thread->state = POK_STATE_RUNNABLE;
			     thread->wakeup_time += POK_GETTICK();
			     if(thread->time_capacity>0)
			       thread->end_time =  thread->wakeup_time + thread->time_capacity;
			   } else {
				 thread->next_activation = thread->wakeup_time + POK_CONFIG_SCHEDULING_MAJOR_FRAME + POK_CURRENT_PARTITION.activation;
				 thread->end_time =  thread->next_activation + thread->time_capacity;
				 thread->state = POK_STATE_WAIT_NEXT_ACTIVATION;
			      
			   }
			 }
		 }
	 }
         pok_sched_stop_thread (pok_partitions[pid].thread_main);
         /* We stop the thread that call this change. All the time,
          * the thread that init this request is the init thread.
          * When it calls this function, the partition is ready and
          * this thread does not need no longer to be executed
          */

         pok_sched ();
         /*
          * Reschedule, baby, reschedule !
          * In fact, the init thread is stopped, we need to execute
          * the other threads.
          */
         break;

#ifdef POK_NEEDS_ERROR_HANDLING
      case POK_PARTITION_MODE_STOPPED:
 
         /*
          * Only the error thread can stop the partition
          */
         if ((POK_CURRENT_PARTITION.thread_error == 0 ) ||
             (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error))
         {
            return POK_ERRNO_PARTITION_MODE;
         }

         pok_partitions[pid].mode = mode;  /* Here, we change the mode */
         pok_sched ();
         break;

      case POK_PARTITION_MODE_INIT_WARM:
      case POK_PARTITION_MODE_INIT_COLD:
	if (pok_partitions[pid].mode == POK_PARTITION_MODE_INIT_COLD && mode == POK_PARTITION_MODE_INIT_WARM)
          { 
             return POK_ERRNO_PARTITION_MODE;
          }

         /*
          * Check that only the error thread can restart the partition
          */
         if ((POK_CURRENT_PARTITION.thread_error == 0 ) ||
             (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error))
         {
            return POK_ERRNO_PARTITION_MODE;
         }

         /*
          * The partition fallback in the INIT_WARM mode when it
          * was in the NORMAL mode. So, we check the previous mode
          */

         pok_partitions[pid].mode = mode;  /* Here, we change the mode */

         pok_partition_reinit (pid);

         pok_sched ();

         break;
#endif

      default:
         return POK_ERRNO_PARTITION_MODE;
         break;
   }
   return POK_ERRNO_OK;
}

/**
 * Change the mode of the current partition (the partition being executed)
 */
pok_ret_t pok_partition_set_mode_current (const pok_partition_mode_t mode)
{
#ifdef POK_NEEDS_ERROR_HANDLING
   if ((POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_main) &&
       (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error))
#else
   if (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_main)
#endif
   {
      return POK_ERRNO_THREAD;
   }

   /*
    * Here, we check which thread call this function.
    * In fact, only two threads can change the partition mode : the init thread
    * and the error thread. If ANY other thread try to change the partition
    * mode, this is an error !
    */
   return (pok_partition_set_mode (POK_SCHED_CURRENT_PARTITION, mode));
}

/**
 * Get partition information. Used for ARINC GET_PARTITION_STATUS function.
 */
pok_ret_t pok_current_partition_get_id (uint8_t *id)
{
  *id = POK_SCHED_CURRENT_PARTITION;
  return POK_ERRNO_OK;
}

pok_ret_t pok_current_partition_get_period (uint64_t *period)
{
  *period = POK_CURRENT_PARTITION.period;
  return POK_ERRNO_OK;
}

pok_ret_t pok_current_partition_get_duration (uint64_t *duration)
{
  *duration = pok_sched_slots[POK_SCHED_CURRENT_PARTITION];
  return POK_ERRNO_OK;
}

pok_ret_t pok_current_partition_get_operating_mode (pok_partition_mode_t *op_mode)
{
  *op_mode = POK_CURRENT_PARTITION.mode;
  return POK_ERRNO_OK;
}

pok_ret_t pok_current_partition_get_lock_level (uint32_t *lock_level)
{
  *lock_level = POK_CURRENT_PARTITION.lock_level;
  return POK_ERRNO_OK;
}

pok_ret_t pok_current_partition_get_start_condition (pok_start_condition_t *start_condition)
{
  *start_condition = POK_CURRENT_PARTITION.start_condition;
  return POK_ERRNO_OK;
}

#ifdef POK_NEEDS_ERROR_HANDLING

/**
 * Stop a thread inside a partition.
 * The \a tid argument is relative to the partition, meaning
 * that it corresponds to a number which bounds are
 * 0 .. number of tasks inside the partition.
 */
pok_ret_t pok_partition_stop_thread (const uint32_t tid)
{
   uint32_t id;
   if (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error)
   {
      return POK_ERRNO_THREAD;
   }

   id = tid + POK_CURRENT_PARTITION.thread_index_low;
   if (POK_CURRENT_PARTITION.thread_index_low > id || POK_CURRENT_PARTITION.thread_index_high < id)
   {
      return POK_ERRNO_THREADATTR;
   }

   /*
    * We check which thread try to call this function. Only the error handling
    * thread can stop other threads.
    */

   pok_sched_stop_thread (id);
   pok_sched ();
   return (POK_ERRNO_OK);
}

/**
 * The \a tid argument is relative to partition thread index
 */
pok_ret_t pok_partition_restart_thread (const uint32_t tid)
{
   uint32_t id;
   if (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error)
   {
      return POK_ERRNO_THREAD;
   }

   id = tid + POK_CURRENT_PARTITION.thread_index_low;
   if (POK_CURRENT_PARTITION.thread_index_low > id || POK_CURRENT_PARTITION.thread_index_high < id)
   {
      return POK_ERRNO_THREADATTR;
   }

   /*
    * We check which thread try to call this function. Only the error handling
    * thread can stop other threads.
    */

   pok_thread_restart (id);
   pok_sched ();
   return (POK_ERRNO_OK);
}
#endif

#endif
