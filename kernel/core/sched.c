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
 **\\file   sched.c
 **\\brief  Function for partitions and kernel scheduling
 **\\author Julien Delange
 */

#if defined (POK_NEEDS_SCHED) || defined (POK_NEEDS_THREADS)

#include <types.h>
#include <arch.h>

#include <core/time.h>
#include <core/sched.h>
#include <core/thread.h>

#ifdef POK_NEEDS_PARTITIONS
#include <core/partition.h>
#endif

#ifdef POK_NEEDS_X86_VMM
#include <core/vcpu.h>
#endif

#ifdef POK_NEEDS_MIDDLEWARE
#include <middleware/port.h>
#endif

#include <dependencies.h>

#include <core/debug.h>
#include <core/instrumentation.h>
#include <core/error.h>

extern pok_thread_t       pok_threads[];

#ifdef POK_NEEDS_PARTITIONS
extern pok_partition_t    pok_partitions[];

/**
 * \brief The variable that contains the value of partition currently being executed
 */
uint8_t                   pok_current_partition;

void                      pok_sched_partition_switch();
#endif

#ifdef POK_NEEDS_X86_VMM

uint8_t                   pok_current_vcpu;
#endif 

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)
void pok_port_flushall (void);
#endif

uint64_t           pok_sched_slots[POK_CONFIG_SCHEDULING_NBSLOTS]
                              = (uint64_t[]) POK_CONFIG_SCHEDULING_SLOTS;
uint8_t           pok_sched_slots_allocation[POK_CONFIG_SCHEDULING_NBSLOTS]
                              = (uint8_t[]) POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION;

pok_sched_t       pok_global_sched;
uint64_t          pok_sched_next_deadline;
uint64_t          pok_sched_next_major_frame;
uint8_t           pok_sched_current_slot = 0; /* Which slot are we executing at this time ?*/
uint32_t	         current_thread = KERNEL_THREAD;

void pok_sched_thread_switch (void);

/**
 *\\brief Init scheduling service
 */

void pok_sched_init (void)
{
#ifdef POK_NEEDS_PARTITIONS 
#if defined (POK_NEEDS_ERROR_HANDLING) || defined (POK_NEEDS_DEBUG)
   /*
    * We check that the total time of time frame
    * corresponds to the sum of each slot
    */
   uint64_t                      total_time;
   uint8_t                       slot;

   total_time = 0;

   for (slot = 0 ; slot < POK_CONFIG_SCHEDULING_NBSLOTS ; slot++)
   {
      total_time = total_time + pok_sched_slots[slot];
   }

   if (total_time != POK_CONFIG_SCHEDULING_MAJOR_FRAME)
   {
#ifdef POK_NEEDS_DEBUG
      printf ("Major frame is not compliant with all time slots\n");
#endif
#ifdef POK_NEEDS_ERROR_HANDLING
      pok_kernel_error (POK_ERROR_KIND_KERNEL_CONFIG);
#endif
   }
#endif
#endif

   pok_sched_current_slot        = 0;
   pok_sched_next_major_frame    = POK_CONFIG_SCHEDULING_MAJOR_FRAME;
   pok_sched_next_deadline       = pok_sched_slots[0];
   pok_current_partition         = pok_sched_slots_allocation[0];
}

uint8_t pok_sched_get_priority_min (const pok_sched_t sched_type)
{
   (void) sched_type;
   /* At this time, we only support one scheduler */
   return 0;
}

uint8_t pok_sched_get_priority_max (const pok_sched_t sched_type)
{
   (void) sched_type;
   /* At this time, we only support one scheduler */
   return 255;
}

#ifdef POK_NEEDS_PARTITIONS
uint8_t	pok_elect_partition()
{
  uint8_t next_partition = POK_SCHED_CURRENT_PARTITION;
# if POK_CONFIG_NB_PARTITIONS > 1
  uint64_t now = POK_GETTICK();

  if (pok_sched_next_deadline <= now)
  {
      /* Here, we change the partition */
#  if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)
    if (pok_sched_next_major_frame <= now)
    {
      pok_sched_next_major_frame = pok_sched_next_major_frame +	POK_CONFIG_SCHEDULING_MAJOR_FRAME;
      pok_port_flushall();
    }
#  endif /* defined (POK_NEEDS_PORTS....) */

    pok_sched_current_slot = (pok_sched_current_slot + 1) % POK_CONFIG_SCHEDULING_NBSLOTS;
    pok_sched_next_deadline = pok_sched_next_deadline + pok_sched_slots[pok_sched_current_slot];
/*
    *  FIXME : current debug session about exceptions-handled
      printf ("Switch from partition %d to partition %d\n", pok_current_partition, pok_sched_current_slot);
      printf ("old current thread = %d\n", POK_SCHED_CURRENT_THREAD);

      printf ("new current thread = %d\n", pok_partitions[pok_sched_current_slot].current_thread);
      printf ("new prev current thread = %d\n", pok_partitions[pok_sched_current_slot].prev_thread);
      */
    next_partition = pok_sched_slots_allocation[pok_sched_current_slot];

#ifdef POK_NEEDS_SCHED_HFPPS
   if (pok_partitions[next_partition].payback > 0) // pay back!
   {
     // new deadline
     pok_sched_next_deadline -= pok_partitions[next_partition].payback;
     pok_partitions[next_partition].payback = 0;
   }
#endif /* POK_NEEDS_SCHED_HFPPS */

  }
# endif /* POK_CONFIG_NB_PARTITIONS > 1 */

  return next_partition;
}
#endif /* POK_NEEDS_PARTITIONS */

#ifdef POK_NEEDS_PARTITIONS
uint32_t	pok_elect_thread(uint8_t new_partition_id)
{
   uint64_t now = POK_GETTICK();
   pok_partition_t* new_partition = &(pok_partitions[new_partition_id]);


   /*
    * We unlock all WAITING threads if the waiting time is passed
    */
   uint8_t i;           /* i is used to browse the partition. We support
                         * only 255 partitions are max, so, we use an uin8_t
                         * type
                         */
   pok_thread_t* thread;
   for (i = 0; i < new_partition->nthreads; i++)
   {
     thread = &(pok_threads[new_partition->thread_index_low + i]);

#if defined (POK_NEEDS_LOCKOBJECTS) || defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)
     if ((thread->state == POK_STATE_WAITING) && (thread->wakeup_time <= now))
     {
       thread->state = POK_STATE_RUNNABLE;
     }
#endif

     if ((thread->state == POK_STATE_WAIT_NEXT_ACTIVATION) && (thread->next_activation <= now))
     {
       thread->state = POK_STATE_RUNNABLE;
       thread->remaining_time_capacity =  thread->time_capacity;
       thread->next_activation = thread->next_activation + thread->period; 
     }
   }

   /*
    * We elect the thread to be executed.
    */
   uint32_t elected;
   switch (new_partition->mode)
   {
      case POK_PARTITION_MODE_INIT_COLD:
      case POK_PARTITION_MODE_INIT_WARM:
#ifdef POK_NEEDS_ERROR_HANDLING
         if ((new_partition->thread_error != 0) &&
             (pok_threads[new_partition->thread_error].state != POK_STATE_STOPPED))
         {
            elected = new_partition->thread_error;
         }
         else
         {
            elected = new_partition->thread_main;
         }
#endif

         elected = new_partition->thread_main;
         break;

      case POK_PARTITION_MODE_NORMAL:
#ifdef POK_NEEDS_ERROR_HANDLING
         if ((new_partition->current_thread == new_partition->thread_error) && 
             (pok_threads[new_partition->current_thread].state == POK_STATE_RUNNABLE))
         {
            elected = new_partition->thread_error;
            break;
         }
#endif
         if ( (POK_SCHED_CURRENT_THREAD != IDLE_THREAD) && 
              (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_main) 
#ifdef POK_NEEDS_ERROR_HANDLING
              && (POK_SCHED_CURRENT_THREAD != POK_CURRENT_PARTITION.thread_error)
#endif
            )
         {
            if (POK_CURRENT_THREAD.remaining_time_capacity > 0)
            {
               POK_CURRENT_THREAD.remaining_time_capacity = POK_CURRENT_THREAD.remaining_time_capacity - 1;
            }
            else
            {
               POK_CURRENT_THREAD.state = POK_STATE_WAIT_NEXT_ACTIVATION;
            }
         }
         elected = new_partition->sched_func (new_partition->thread_index_low,
                                                     new_partition->thread_index_high,
						     new_partition->prev_thread,
						     new_partition->current_thread);
#ifdef POK_NEEDS_INSTRUMENTATION
          if ( (elected != IDLE_THREAD) && (elected != new_partition->thread_main))
          {
            pok_instrumentation_running_task (elected);
          }
#endif

         break;

      default:
         elected = IDLE_THREAD;
         break;
   }

#ifdef POK_NEEDS_SCHED_HFPPS
   if (pok_threads[elected].payback > 0) // pay back!
   {
     pok_threads[elected].remaining_time_capacity -= pok_threads[elected].payback;
     pok_threads[elected].payback = 0;
   }
#endif /* POK_NEEDS_SCHED_HFPPS */

   // computed next thread's deadline
   pok_threads[elected].end_time = now + pok_threads[elected].remaining_time_capacity;

   return (elected);
}
#endif /* POK_NEEDS_PARTITIONS */

#ifdef POK_NEEDS_PARTITIONS
void pok_sched()
{
  uint32_t elected_thread = 0;
  uint8_t elected_partition = POK_SCHED_CURRENT_PARTITION;

#ifdef POK_NEEDS_SCHED_HFPPS
  uint64_t now = POK_GETTICK();
  elected_thread = current_thread;

  /* if thread hasn't finished its job and its deadline is passed */
  if (pok_threads[elected_thread].end_time <= now && pok_threads[elected_thread].remaining_time_capacity > 0)
  {
    /* updates thread and partition payback */
    pok_threads[elected_thread].payback = pok_threads[elected_thread].remaining_time_capacity;
    pok_partitions[pok_current_partition].payback = pok_threads[elected_thread].remaining_time_capacity;
    /* computes next partition deadline */
    pok_sched_next_deadline += pok_threads[elected_thread].remaining_time_capacity;
  }
  else /* overmegadirty */
#endif /* POK_NEEDS_SCHED_HFPPS */
  {
  
    elected_partition = pok_elect_partition();
    elected_thread = pok_elect_thread(elected_partition);
  }

   pok_current_partition = elected_partition;
   if(pok_partitions[pok_current_partition].current_thread != elected_thread) {
	   if(pok_partitions[pok_current_partition].current_thread != IDLE_THREAD) {
		   pok_partitions[pok_current_partition].prev_thread = pok_partitions[pok_current_partition].current_thread;
	   }
	   pok_partitions[pok_current_partition].current_thread = elected_thread;
   }
  pok_sched_context_switch(elected_thread);
}
#else
void pok_sched_thread_switch ()
{
   int i;
   uint64_t now;
   uint32_t elected;

   now = POK_GETTICK();
   for (i = 0; i <= POK_CONFIG_NB_THREADS; ++i)
   {
     if ((pok_threads[i].state == POK_STATE_WAITING) &&
	 (pok_threads[i].wakeup_time <= now))
      {
         pok_threads[i].state = POK_STATE_RUNNABLE;
      }
   }

   elected = pok_sched_part_election (0, POK_CONFIG_NB_THREADS);
   /*
    *  FIXME : current debug session about exceptions-handled
   printf ("switch to thread %d\n", elected);
   */
   pok_sched_context_switch(elected);
#ifdef POK_NEEDS_X86_VMM
   upcall_irq();
}
#endif /* POK_NEEDS_PARTITIONS */

/*
 * Context-switch function to switch from one thread to another
 * Rely on architecture-dependent functionnalities (must include arch.h)
 */
void pok_sched_context_switch (const uint32_t elected_id)
{
   uint32_t *current_sp;
   uint32_t new_sp;

   if (POK_SCHED_CURRENT_THREAD == elected_id)
   {
      return;
   }

   current_sp = &POK_CURRENT_THREAD.sp;
   new_sp = pok_threads[elected_id].sp;
/*
    *  FIXME : current debug session about exceptions-handled
   printf("switch from thread %d, sp=0x%x\n",POK_SCHED_CURRENT_THREAD, current_sp);
   printf("switch to thread %d, sp=0x%x\n",elected_id, new_sp);
   */
   pok_space_switch(POK_CURRENT_THREAD.partition,
		    pok_threads[elected_id].partition);

   current_thread = elected_id;

   pok_context_switch(current_sp, new_sp);
}

#ifdef POK_NEEDS_SCHED_RMS
uint32_t pok_sched_part_rms (const uint32_t index_low, const uint32_t index_high,const uint32_t __attribute__((unused)) prev_thread,const uint32_t __attribute__((unused)) current_thread)
{
   uint32_t res;
#ifdef POK_NEEDS_DEBUG
   uint32_t from;
   from = prev_thread;
#endif

   res= index_low;

   do
   {
      res++;
      if (res >= index_high)
      {
         res = index_low;
      }
   }
   while ((res != index_low) &&
	  (pok_threads[res].state != POK_STATE_RUNNABLE));

   if ((res == index_low) && (pok_threads[res].state != POK_STATE_RUNNABLE))
   {
      res = IDLE_THREAD;
   }

#ifdef POK_NEEDS_DEBUG
    if ( res!= IDLE_THREAD)
    {
        printf("--- scheduling thread: %d {%d} --- ", res,
	       pok_threads[res].period);
        from=index_low;
        while ( from <= index_high )
        {
            if ( pok_threads[from].state==POK_STATE_RUNNABLE )
            {
                printf(" %d {%d} ,",from,pok_threads[from].period);
            }
            from++;
        }
        printf(" are runnable\n");
    }
#endif

   return res;
}
#endif /* POK_NEEDS_SCHED_RMS */


uint32_t pok_sched_part_rr (const uint32_t index_low, const uint32_t index_high,const uint32_t prev_thread,const uint32_t current_thread)
{
   uint32_t res;
   uint32_t from;

   if (current_thread == IDLE_THREAD)
   {
      res = prev_thread;
   }
   else
   {
      res = current_thread;
   }

   from = res;

   if ((pok_threads[current_thread].remaining_time_capacity > 0) && (pok_threads[current_thread].state == POK_STATE_RUNNABLE))
   {
      return current_thread;
   }

   do
   {
      res++;
      if (res > index_high)
      {
         res = index_low;
      }
   }
   while ((res != from) && (pok_threads[res].state != POK_STATE_RUNNABLE));

   if ((res == from) && (pok_threads[res].state != POK_STATE_RUNNABLE))
   {
      res = IDLE_THREAD;
   }
   return res;
}


#if defined (POK_NEEDS_LOCKOBJECTS) || defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)
void pok_sched_unlock_thread (const uint32_t thread_id)
{
   pok_threads[thread_id].state = POK_STATE_RUNNABLE;
}
#endif

#if defined (POK_NEEDS_LOCKOBJECTS) || defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)
void pok_sched_lock_current_thread (void)
{
   pok_threads[current_thread].state = POK_STATE_LOCK;
}

void pok_sched_lock_current_thread_timed (const uint64_t time)
{
   pok_threads[current_thread].state = POK_STATE_WAITING;
   pok_threads[current_thread].wakeup_time = time;
}
#endif

#ifdef POK_NEEDS_SCHED_STOP_SELF
void pok_sched_stop_self (void)
{
   POK_CURRENT_THREAD.state = POK_STATE_STOPPED;
   pok_sched ();
}
#endif

void pok_sched_stop_thread (const uint32_t tid)
{
   pok_threads[tid].state = POK_STATE_STOPPED;
}

#ifdef POK_NEEDS_DEPRECIATED
void pok_sched_lock_thread (const uint32_t thread_id)
{
   pok_threads[thread_id].state = POK_STATE_LOCK;
}
#endif

pok_ret_t pok_sched_end_period ()
{
   POK_CURRENT_THREAD.state = POK_STATE_WAIT_NEXT_ACTIVATION;
   POK_CURRENT_THREAD.remaining_time_capacity = 0;
   pok_sched ();
   return POK_ERRNO_OK;
}

#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)
void pok_sched_activate_error_thread (void)
{
   uint32_t error_thread = pok_partitions[pok_current_partition].thread_error;
   if (error_thread != 0)
   {
      pok_threads[error_thread].remaining_time_capacity = 1000;
      pok_threads[error_thread].period = 100;
      pok_threads[error_thread].next_activation= 0;

      pok_threads[error_thread].state  = POK_STATE_RUNNABLE;
      pok_sched_context_switch (error_thread);
   }
}
#endif

#ifdef POK_NEEDS_PARTITIONS

uint32_t pok_sched_get_current(uint32_t *thread_id)
{
#if defined (POK_NEEDS_ERROR_HANDLING)
  if (pok_partitions[pok_current_partition].thread_error == 0)
    return POK_ERRNO_THREAD;
#endif
  if (KERNEL_THREAD == POK_SCHED_CURRENT_THREAD 
      || IDLE_THREAD == POK_SCHED_CURRENT_THREAD)
    {
      return POK_ERRNO_THREAD;
    }
  *thread_id=POK_SCHED_CURRENT_THREAD;
  return POK_ERRNO_OK;
}
#endif

#ifdef POK_NEEDS_X86_VMM
/*
 * For now, as we do not need schedule of vcpu.
 */
#define switch_kerenl_stack(v) ((void)0)
#define save_segments(p) ((void)0)
#define load_segments(n) ((void)0)
int sched_init_vcpu(vcpu_t *v)
{
  v->sched_info.start_time = POK_GETTICK();
  return POK_ERRNO_OK;
}

void sched_ctxt_switch_from_vcpu(vcpu_t *v)
{
  // To avoid the compile error, add this statement.
  save_segments(v);
  vcpu_t *vcpu;
  vcpu = v;
  v = vcpu;
  return;
}
void sched_ctxt_switch_to_vcpu(vcpu_t *v)
{
  // To avoid the compile error, add this statement.
  load_segments(v);
  vcpu_t *vcpu;
  vcpu = v;
  v = vcpu;
  return;
}

void sched_tail_vcpu(vcpu_t *v)
{
  // To avoid the compile error, add this statement.
  vcpu_t *vcpu;
  vcpu = v;
  v = vcpu;
  return; 
}
#endif /* POK_NEEDS_X86_VMM */
#endif /* __POK_NEEDS_SCHED */
