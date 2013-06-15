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
 * \file    core/lockobj.c
 * \brief   Provides functionnalities for locking functions (mutexes, semaphores and so on)
 * \author  Julien Delange
 *
 * This file contains the implementation code for mutexes, conditions and
 * semaphores. This is implemented in the same file since the functionnalities
 * does not differ so much.
 */


#if defined (POK_NEEDS_LOCKOBJECTS) || defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)

#include <arch.h>
#include <errno.h>
#include <types.h>
#include <core/sched.h>
#include <core/time.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/lockobj.h>
#include <libc.h>

pok_lockobj_t           pok_partitions_lockobjs[POK_CONFIG_NB_LOCKOBJECTS+1];

/**
 * Init the array of lockobjects
 */
pok_ret_t pok_lockobj_init ()
{
#if POK_CONFIG_NB_LOCKOBJECTS > 0
   uint8_t i;

#ifdef POK_NEEDS_PARTITIONS
#ifdef POK_NEEDS_ERROR_HANDLING
   uint32_t total_lockobjects;

   total_lockobjects = 0;

   for ( i = 0 ; i < POK_CONFIG_NB_PARTITIONS ; i++)
   {
      total_lockobjects = total_lockobjects + pok_partitions[i].nlockobjs;
   }
   
   if (total_lockobjects != POK_CONFIG_NB_LOCKOBJECTS)
   {
      pok_kernel_error (POK_ERROR_KIND_KERNEL_CONFIG);
   }
#endif
#endif

   for ( i = 0 ; i < POK_CONFIG_NB_LOCKOBJECTS ; i++)
   {
      pok_partitions_lockobjs[i].spin        = 0;
      pok_partitions_lockobjs[i].is_locked   = FALSE;
      pok_partitions_lockobjs[i].initialized = FALSE;
   }
#endif
   return POK_ERRNO_OK;
}


pok_ret_t pok_lockobj_create (pok_lockobj_t* obj, const pok_lockobj_attr_t* attr)
{
   uint32_t tmp;

   /* Check the policy of the lockobj */
   if ((attr->locking_policy != POK_LOCKOBJ_POLICY_STANDARD) && (attr->locking_policy != POK_LOCKOBJ_POLICY_PIP) && (attr->locking_policy != POK_LOCKOBJ_POLICY_PCP))
   {
      return POK_ERRNO_LOCKOBJ_POLICY;
   }
   
   /* Check the kind of the locjobj, must have a declared kind
    * If not, of course, we reject the creation.
    */
   if ((attr->kind != POK_LOCKOBJ_KIND_MUTEX) && (attr->kind != POK_LOCKOBJ_KIND_SEMAPHORE) && (attr->kind != POK_LOCKOBJ_KIND_EVENT))
   {
      return POK_ERRNO_LOCKOBJ_KIND;
   }

   for (tmp = 0 ; tmp < POK_CONFIG_NB_THREADS ; tmp++ )
   {
      obj->thread_state [tmp] = LOCKOBJ_STATE_UNLOCK;
   }
 
   obj->queueing_policy = attr->queueing_policy;
   obj->locking_policy  = attr->locking_policy;
   obj->kind            = attr->kind;
   obj->initialized     = TRUE;

   if (attr->kind == POK_LOCKOBJ_KIND_SEMAPHORE)
   {
      obj->current_value = attr->initial_value;
      obj->max_value     = attr->max_value;
      
      if (obj->current_value == 0)
      {
         obj->is_locked = TRUE;
      }
   }

   return POK_ERRNO_OK;
}

#ifdef POK_NEEDS_LOCKOBJECTS
pok_ret_t pok_lockobj_partition_create (pok_lockobj_id_t* id, const pok_lockobj_attr_t* attr)
{
   uint8_t i;
   uint8_t pid;
   uint8_t mid;
   pok_ret_t ret;
   uint8_t lower_bound = 0;
   uint8_t upper_bound = 0;
   bool_t  found = FALSE;

   if (  (POK_CURRENT_PARTITION.mode != POK_PARTITION_MODE_INIT_COLD) &&
         (POK_CURRENT_PARTITION.mode != POK_PARTITION_MODE_INIT_WARM))
   {
      return POK_ERRNO_MODE;
   }
   
   pid = POK_SCHED_CURRENT_PARTITION;

   lower_bound = pok_partitions[pid].lockobj_index_low;
   upper_bound = pok_partitions[pid].lockobj_index_high;

   /*
    * Find a lockobject for the partition
    */
   mid = lower_bound;
   while (mid < upper_bound)
   {
      if (pok_partitions_lockobjs[mid].initialized == FALSE)
      {
         found = TRUE; /* Yeeepeee, we found a free lockobj for this partition */
         break;
      }
      mid++;
   }

   if (found == FALSE)
   {
      return POK_ERRNO_LOCKOBJ_UNAVAILABLE;
   }

   *id = mid;

   ret = pok_lockobj_create (&pok_partitions_lockobjs[mid], attr);

   if (ret != POK_ERRNO_OK)
   {
      return ret;
   }

   for (i = 0 ; i < POK_CONFIG_NB_THREADS ; i++)
   {
      pok_partitions_lockobjs[mid].thread_state[i] = LOCKOBJ_STATE_UNLOCK;
   }

   return POK_ERRNO_OK;
}
#endif

pok_ret_t pok_lockobj_eventwait (pok_lockobj_t* obj, const uint64_t timeout)
{
   pok_ret_t ret;

   SPIN_LOCK (obj->eventspin);

   if (obj->initialized == FALSE)
   {
      return POK_ERRNO_LOCKOBJ_NOTREADY;
   }

   if (obj->kind != POK_LOCKOBJ_KIND_EVENT)
   {
      return POK_ERRNO_EINVAL;
   }

   if (pok_lockobj_unlock (obj, NULL))
   {
      SPIN_UNLOCK (obj->eventspin);
      return POK_ERRNO_UNAVAILABLE;
   }

   obj->thread_state[POK_SCHED_CURRENT_THREAD] = LOCKOBJ_STATE_WAITEVENT;

   if (timeout > 0)
   {
      pok_sched_lock_current_thread_timed (timeout);
   }
   else
   {
      pok_sched_lock_current_thread ();
   }

   SPIN_UNLOCK (obj->eventspin);
   pok_sched ();
   obj->thread_state[POK_SCHED_CURRENT_THREAD] = LOCKOBJ_STATE_UNLOCK;

   ret = pok_lockobj_lock (obj, NULL);

   if (ret != POK_ERRNO_OK)
   {
      SPIN_UNLOCK (obj->eventspin);
      return ret;
   }

   /* Here, we come back after we wait*/
   if ((timeout != 0 ) && (POK_GETTICK() >= timeout))
   {
      ret = POK_ERRNO_TIMEOUT;
   }
   else
   {
      ret = POK_ERRNO_OK;
   }

   SPIN_UNLOCK (obj->eventspin);

   return ret;
}

pok_ret_t pok_lockobj_eventsignal (pok_lockobj_t* obj)
{
   SPIN_LOCK (obj->eventspin);
   uint32_t tmp;

   for (tmp = 0 ; tmp < POK_CONFIG_NB_THREADS ; tmp++)
   {
      if (tmp == POK_SCHED_CURRENT_THREAD)
         continue;
      
      if (obj->thread_state[tmp] == LOCKOBJ_STATE_WAITEVENT)
      {
         pok_sched_unlock_thread (tmp);
         SPIN_UNLOCK (obj->eventspin);
         return POK_ERRNO_OK;
      }
 
   }
   SPIN_UNLOCK (obj->eventspin);
   return POK_ERRNO_NOTFOUND;
}

pok_ret_t pok_lockobj_eventbroadcast (pok_lockobj_t* obj)
{
   uint32_t tmp;
   SPIN_LOCK (obj->eventspin);

   for (tmp = 0 ; tmp < POK_CONFIG_NB_THREADS ; tmp++)
   {
      if (tmp == POK_SCHED_CURRENT_THREAD)
         continue;
      
      if (obj->thread_state[tmp] == LOCKOBJ_STATE_WAITEVENT)
      {
         pok_sched_unlock_thread (tmp);
      }
 
   }

   SPIN_UNLOCK (obj->eventspin);

   return POK_ERRNO_OK;
}


pok_ret_t pok_lockobj_lock (pok_lockobj_t* obj, const pok_lockobj_lockattr_t* attr)
{
   uint64_t timeout = 0;

   if (obj->initialized == FALSE)
   {
      return POK_ERRNO_LOCKOBJ_NOTREADY;
   }
   
   SPIN_LOCK (obj->spin);

   if ( (obj->is_locked == FALSE ) && (obj->thread_state[POK_SCHED_CURRENT_THREAD] == LOCKOBJ_STATE_UNLOCK ))
   {
      obj->is_locked = TRUE;
      SPIN_UNLOCK (obj->spin);
   }
   else
   {
      /*
       * attr->time corresponds to the timeout for the waiting object
       */
      if ((attr != NULL) && (attr->time > 0))
      {
         timeout = attr->time;
      }

      while ( (obj->is_locked == TRUE ) || (obj->thread_state[POK_SCHED_CURRENT_THREAD] == LOCKOBJ_STATE_LOCK)) 
      {
         obj->thread_state[POK_SCHED_CURRENT_THREAD] = LOCKOBJ_STATE_LOCK;

         if (timeout > 0)
         {
            pok_sched_lock_current_thread_timed (timeout);
            if (POK_GETTICK() >= timeout)
            {
               obj->thread_state[POK_SCHED_CURRENT_THREAD] = LOCKOBJ_STATE_UNLOCK;
               return POK_ERRNO_TIMEOUT;
            }
         }
         else
         {
            pok_sched_lock_current_thread ();
         }
         
         SPIN_UNLOCK (obj->spin);
         pok_sched();     /* reschedule baby, reschedule !! */
      }
      
      switch (obj->kind)
      {
         case POK_LOCKOBJ_KIND_SEMAPHORE:
         {
            obj->current_value--;
            if (obj->current_value == 0)
            {
               obj->is_locked = TRUE;
            }
            break;
         }
         
         case POK_LOCKOBJ_KIND_MUTEX:
         {
            obj->is_locked = TRUE;
            break;
         }
         
         default:
         {
            obj->is_locked = TRUE;
            break;
         }
      }
      pok_sched_unlock_thread (POK_SCHED_CURRENT_THREAD);
   }

   return POK_ERRNO_OK;
}


pok_ret_t pok_lockobj_unlock (pok_lockobj_t* obj, const pok_lockobj_lockattr_t* attr)
{
   uint32_t res;

   (void) attr;         /* unused at this time */
   
   if (obj->initialized == FALSE)
   {
      return POK_ERRNO_LOCKOBJ_NOTREADY;
   }
   
   res = 0;
   SPIN_LOCK (obj->spin);

   switch (obj->kind)
   {
      case POK_LOCKOBJ_KIND_SEMAPHORE:
      {
         if (obj->current_value < obj->max_value)
         {
            obj->current_value++;
         }
         obj->is_locked = FALSE;
         break;
      }
      
      case POK_LOCKOBJ_KIND_MUTEX:
      {
         obj->is_locked = FALSE;
         break;
      }
      
      default:
      {
         obj->is_locked = FALSE;
         break;
      }
   }  
   
   res = POK_SCHED_CURRENT_THREAD;
   res = (res + 1) % (POK_CONFIG_NB_THREADS);

   do
   { 
      if (obj->thread_state[res] == LOCKOBJ_STATE_LOCK)
      { 
         obj->thread_state[res] = LOCKOBJ_STATE_UNLOCK;
         pok_sched_unlock_thread (res);
         break;
      }  
      res = (res + 1) % (POK_CONFIG_NB_THREADS);
   }
   while ((res != POK_SCHED_CURRENT_THREAD));

   obj->thread_state[POK_SCHED_CURRENT_THREAD] = LOCKOBJ_STATE_UNLOCK;
   SPIN_UNLOCK (obj->spin);

   return POK_ERRNO_OK;
}

#ifdef POK_NEEDS_LOCKOBJECTS
pok_ret_t pok_lockobj_partition_wrapper (const pok_lockobj_id_t id, const pok_lockobj_lockattr_t* attr)
{
   /* First, we check that the locked object belongs to the partition
    * If not, we return an error
    */
   pok_ret_t ret;

   if (id < pok_partitions[POK_SCHED_CURRENT_PARTITION].lockobj_index_low)
   {
      return POK_ERRNO_EINVAL;
   }
   
   if ( id >= pok_partitions[POK_SCHED_CURRENT_PARTITION].lockobj_index_high)
   {
      return POK_ERRNO_EINVAL;
   }

   if (pok_partitions_lockobjs[id].kind != attr->obj_kind)
   {
      return POK_ERRNO_EINVAL;
   }

   switch (attr->operation)
   {
      case LOCKOBJ_OPERATION_LOCK:
         ret = pok_lockobj_lock (&pok_partitions_lockobjs[id], attr);
         return ret;
         break;

      case LOCKOBJ_OPERATION_UNLOCK:
         {
            ret = pok_lockobj_unlock (&pok_partitions_lockobjs[id], attr);
            return ret;
            break;
         }

      case LOCKOBJ_OPERATION_WAIT:
         {
            ret = pok_lockobj_eventwait (&pok_partitions_lockobjs[id], attr->time);
            return ret;
            break;
         }

      case LOCKOBJ_OPERATION_SIGNAL:
         {
            ret = pok_lockobj_eventsignal (&pok_partitions_lockobjs[id]);
            break;
         }

      case LOCKOBJ_OPERATION_BROADCAST:
         {
            ret = pok_lockobj_eventbroadcast (&pok_partitions_lockobjs[id]);
            break;
         }

      default:
         return POK_ERRNO_EINVAL;
   }
   return POK_ERRNO_EINVAL;
}
#endif

#endif

