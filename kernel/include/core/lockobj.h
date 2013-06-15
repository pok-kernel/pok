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


#ifndef __POK_KERNEL_LOCKOBJ_H__
#define __POK_KERNEL_LOCKOBJ_H__

#include <types.h>
#include <arch.h>

#ifndef POK_CONFIG_NB_LOCKOBJECTS
#define POK_CONFIG_NB_LOCKOBJECTS 0
#endif

/*
 * In POK, all locking objects are implemented in the same way. It avoids
 * code duplication and save the environnement because we will consume less
 * space on your disk and will save trees because stupid people that prints
 * the code will consume less paper.
 * Moreoever, if all lockobjects share the same code, it will be easy to
 * certify and verify the behavior of all.
 */


typedef enum
{
   POK_LOCKOBJ_KIND_MUTEX = 1,
   POK_LOCKOBJ_KIND_SEMAPHORE = 2,
   POK_LOCKOBJ_KIND_EVENT = 3
}pok_lockobj_kind_t;

/* All kind of lock objects we have in the kernel */

typedef enum
{
   POK_LOCKOBJ_POLICY_STANDARD = 0,
   POK_LOCKOBJ_POLICY_PIP      = 1,
   POK_LOCKOBJ_POLICY_PCP      = 2
}pok_locking_policy_t;


typedef enum
{
   LOCKOBJ_STATE_LOCK = 0,
   LOCKOBJ_STATE_UNLOCK = 1,
   LOCKOBJ_STATE_WAITEVENT = 2
}pok_mutex_state_t;


typedef struct
{
   pok_lockobj_kind_t         kind;
   pok_locking_policy_t       locking_policy;
   pok_queueing_discipline_t  queueing_policy;  
   pok_sem_value_t            initial_value;
   pok_sem_value_t            max_value;
}pok_lockobj_attr_t;

typedef struct
{
   pok_spinlock_t             spin;
   pok_spinlock_t             eventspin;
   /* spinlock to enfoce mutual exclusion */

   bool_t                     is_locked;
   /* Is the mutex locked ? */
   
   pok_mutex_state_t          thread_state[POK_CONFIG_NB_THREADS + 2];
   /* Describe which thread is blocked in the mutex */
   
   pok_locking_policy_t       locking_policy;
   /* Locking policy */

   pok_queueing_discipline_t  queueing_policy;
   /* Locking policy */

   pok_lockobj_kind_t         kind;
   
   bool_t                     initialized;
   /* Is the mutex initialized ? */
   
   uint16_t                   current_value;
   uint16_t                   max_value;
} pok_lockobj_t;


typedef enum
{
   LOCKOBK_LOCK_REGULAR = 1,
   LOCKOBJ_LOCK_TIMED   = 2
}pok_lockobj_lock_kind_t;

typedef enum
{
   LOCKOBJ_OPERATION_LOCK = 1,
   LOCKOBJ_OPERATION_UNLOCK = 2,
   LOCKOBJ_OPERATION_WAIT = 3,
   LOCKOBJ_OPERATION_SIGNAL = 4,
   LOCKOBJ_OPERATION_BROADCAST = 5
}pok_lockobj_operation_t;

typedef struct
{
   pok_lockobj_operation_t    operation;
   pok_lockobj_kind_t         obj_kind;
   pok_lockobj_lock_kind_t    lock_kind;
   uint64_t                   time;
}pok_lockobj_lockattr_t;


pok_ret_t pok_lockobj_create (pok_lockobj_t* obj, const pok_lockobj_attr_t* attr);
pok_ret_t pok_lockobj_init ();
pok_ret_t pok_lockobj_partition_create (pok_lockobj_id_t* id, const pok_lockobj_attr_t* attr);
pok_ret_t pok_lockobj_lock (pok_lockobj_t* obj, const pok_lockobj_lockattr_t* attr);
pok_ret_t pok_lockobj_unlock (pok_lockobj_t* obj, const pok_lockobj_lockattr_t* attr);
pok_ret_t pok_lockobj_eventwait (pok_lockobj_t* obj, const uint64_t timeout);
pok_ret_t pok_lockobj_eventsignal (pok_lockobj_t* obj);
pok_ret_t pok_lockobj_eventbroadcast (pok_lockobj_t* obj);
pok_ret_t pok_lockobj_partition_wrapper (const pok_lockobj_id_t id, const pok_lockobj_lockattr_t* attr);

#endif
