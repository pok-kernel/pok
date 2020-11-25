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
 *                                      Copyright (c) 2007-2020 POK team
 */

/**
 * \file    core/lockobj.c
 * \brief   Provides functionnalities for locking functions (mutexes, semaphores
 * and so on) \author  Julien Delange
 *
 * This file contains the implementation code for mutexes, conditions and
 * semaphores. This is implemented in the same file since the functionnalities
 * does not differ so much.
 */

#if defined(POK_NEEDS_LOCKOBJECTS) || defined(POK_NEEDS_PORTS_QUEUEING) ||     \
    defined(POK_NEEDS_PORTS_SAMPLING)

#include <arch.h>
#include <assert.h>
#include <core/lockobj.h>
#include <core/partition.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/time.h>
#include <errno.h>
#include <libc.h>
#include <types.h>

pok_lockobj_t pok_partitions_lockobjs[POK_CONFIG_NB_LOCKOBJECTS + 1];

/**
 * Init the array of lockobjects
 */
pok_ret_t pok_lockobj_init() {
#if POK_CONFIG_NB_LOCKOBJECTS > 0
  uint8_t i;

  uint32_t total_lockobjects;

  total_lockobjects = 0;

  for (i = 0; i < POK_CONFIG_NB_PARTITIONS; i++) {
    total_lockobjects = total_lockobjects + pok_partitions[i].nlockobjs;
  }

  if (total_lockobjects != POK_CONFIG_NB_LOCKOBJECTS) {
    pok_kernel_error(POK_ERROR_KIND_KERNEL_CONFIG);
  }

  for (i = 0; i < POK_CONFIG_NB_LOCKOBJECTS; i++) {
    pok_partitions_lockobjs[i].spin = 0;
    pok_partitions_lockobjs[i].current_value = 1;
    pok_partitions_lockobjs[i].initialized = FALSE;
  }
#endif
  return POK_ERRNO_OK;
}

pok_ret_t pok_lockobj_create(pok_lockobj_t *obj,
                             const pok_lockobj_attr_t *attr) {

  /* Check the policy of the lockobj */
  if ((attr->locking_policy != POK_LOCKOBJ_POLICY_STANDARD) &&
      (attr->locking_policy != POK_LOCKOBJ_POLICY_PIP) &&
      (attr->locking_policy != POK_LOCKOBJ_POLICY_PCP)) {
    return POK_ERRNO_LOCKOBJ_POLICY;
  }

  /* Check the kind of the locjobj, must have a declared kind
   * If not, of course, we reject the creation.
   */
  if ((attr->kind != POK_LOCKOBJ_KIND_MUTEX) &&
      (attr->kind != POK_LOCKOBJ_KIND_SEMAPHORE) &&
      (attr->kind != POK_LOCKOBJ_KIND_EVENT)) {
    return POK_ERRNO_LOCKOBJ_KIND;
  }

  pok_lockobj_fifo_init(&obj->fifo);
  pok_lockobj_fifo_init(&obj->event_fifo);

  if (attr->kind == POK_LOCKOBJ_KIND_EVENT) {
    obj->queueing_policy = POK_QUEUEING_DISCIPLINE_PRIORITY;
  } else {
    obj->queueing_policy = attr->queueing_policy;
  }
  obj->locking_policy = attr->locking_policy;
  obj->kind = attr->kind;
  obj->initialized = TRUE;

  if (attr->kind == POK_LOCKOBJ_KIND_SEMAPHORE) {
    obj->current_value = attr->initial_value;
    obj->max_value = attr->max_value;
  } else {
    obj->current_value = 1;
  }

  return POK_ERRNO_OK;
}

#ifdef POK_NEEDS_LOCKOBJECTS
pok_ret_t pok_lockobj_partition_create(pok_lockobj_id_t *id,
                                       const pok_lockobj_attr_t *attr) {
  uint8_t pid;
  uint8_t mid;
  pok_ret_t ret;
  uint8_t lower_bound = 0;
  uint8_t upper_bound = 0;
  bool_t found = FALSE;

  if ((POK_CURRENT_PARTITION.mode != POK_PARTITION_MODE_INIT_COLD) &&
      (POK_CURRENT_PARTITION.mode != POK_PARTITION_MODE_INIT_WARM)) {
    return POK_ERRNO_MODE;
  }

  pid = POK_SCHED_CURRENT_PARTITION;

  lower_bound = pok_partitions[pid].lockobj_index_low;
  upper_bound = pok_partitions[pid].lockobj_index_high;

  /*
   * Find a lockobject for the partition
   */
  mid = lower_bound;
  while (mid < upper_bound) {
    if (pok_partitions_lockobjs[mid].initialized == FALSE) {
      found = TRUE; /* Yeeepeee, we found a free lockobj for this partition */
      break;
    }
    mid++;
  }

  if (found == FALSE) {
    return POK_ERRNO_LOCKOBJ_UNAVAILABLE;
  }

  *id = mid;

  ret = pok_lockobj_create(&pok_partitions_lockobjs[mid], attr);

  if (ret != POK_ERRNO_OK) {
    return ret;
  }

  return POK_ERRNO_OK;
}
#endif

pok_ret_t pok_lockobj_eventwait(pok_lockobj_t *obj, uint64_t timeout) {
  SPIN_LOCK(obj->eventspin);

  if (obj->initialized == FALSE) {
    SPIN_UNLOCK(obj->eventspin);
    return POK_ERRNO_LOCKOBJ_NOTREADY;
  }

  if (obj->kind != POK_LOCKOBJ_KIND_EVENT) {
    SPIN_UNLOCK(obj->eventspin);
    return POK_ERRNO_EINVAL;
  }

  if (pok_lockobj_unlock(obj, NULL)) {
    SPIN_UNLOCK(obj->eventspin);
    return POK_ERRNO_UNAVAILABLE;
  }
#ifdef POK_NEEDS_ASSERT
  pok_ret_t ret =
      pok_lockobj_enqueue(&obj->event_fifo, POK_SCHED_CURRENT_THREAD,
                          POK_QUEUEING_DISCIPLINE_PRIORITY);
  assert(!ret);
#else
  pok_lockobj_enqueue(&obj->event_fifo, POK_SCHED_CURRENT_THREAD,
                      POK_QUEUEING_DISCIPLINE_PRIORITY);
#endif
  uint64_t deadline = timeout ? timeout + POK_GETTICK() : 0;

  if (deadline > 0) {
    pok_sched_lock_current_thread_timed(deadline);
  } else {
    pok_sched_lock_current_thread();
  }

  SPIN_UNLOCK(obj->eventspin);
  pok_sched_thread(TRUE);

  pok_ret_t ret_wait;
  /* Here, we come back after we wait*/
  if ((deadline != 0) && (POK_GETTICK() >= deadline)) {
    ret_wait = POK_ERRNO_TIMEOUT;
    SPIN_LOCK(obj->eventspin);
    pok_lockobj_remove_thread(&obj->event_fifo, POK_SCHED_CURRENT_THREAD);
    SPIN_UNLOCK(obj->eventspin);
  } else {
    ret_wait = POK_ERRNO_OK;
  }

  pok_ret_t ret_lock = pok_lockobj_lock(obj, NULL);

  if (ret_lock != POK_ERRNO_OK)
    return ret_lock;
  else
    return ret_wait;
}

pok_ret_t pok_lockobj_eventsignal(pok_lockobj_t *obj) {
  SPIN_LOCK(obj->eventspin);
  uint32_t tmp;

  if (pok_lockobj_fifo_is_empty(&obj->event_fifo)) {
    SPIN_UNLOCK(obj->eventspin);
    return POK_ERRNO_NOTFOUND;
  } else {
    tmp = pok_lockobj_get_head(&obj->event_fifo);
    pok_sched_unlock_thread(tmp);
    pok_lockobj_dequeue(&obj->event_fifo);
    SPIN_UNLOCK(obj->eventspin);
    pok_threads_schedule_one_proc(pok_threads[tmp].processor_affinity);
    return POK_ERRNO_OK;
  }
}

pok_ret_t pok_lockobj_eventbroadcast(pok_lockobj_t *obj) {
  uint32_t tmp;
  bool_t resched = FALSE;
  SPIN_LOCK(obj->eventspin);

  while (!pok_lockobj_fifo_is_empty(&obj->event_fifo)) {
    tmp = pok_lockobj_get_head(&obj->event_fifo);
    pok_sched_unlock_thread(tmp);
    pok_lockobj_dequeue(&obj->event_fifo);
    resched = TRUE;
  }

  SPIN_UNLOCK(obj->eventspin);
  if (resched) {
    pok_threads_schedule_every_proc();
  }

  return POK_ERRNO_OK;
}

pok_ret_t pok_lockobj_lock(pok_lockobj_t *obj,
                           const pok_lockobj_lockattr_t *attr) {
  if (obj->initialized == FALSE) {
    return POK_ERRNO_LOCKOBJ_NOTREADY;
  }
  SPIN_LOCK(obj->spin);

  if (obj->current_value > 0) {
    // Short path: object is available right now
    assert(pok_lockobj_fifo_is_empty(&obj->fifo));
    obj->current_value--;
    SPIN_UNLOCK(obj->spin);
    return POK_ERRNO_OK;
  } else {
    uint64_t deadline =
        attr != NULL && attr->timeout > 0 ? attr->timeout + POK_GETTICK() : 0;
    pok_lockobj_enqueue(&obj->fifo, POK_SCHED_CURRENT_THREAD,
                        obj->queueing_policy);
    if (deadline > 0)
      pok_sched_lock_current_thread_timed(deadline);
    else
      pok_sched_lock_current_thread();

    SPIN_UNLOCK(obj->spin);
    pok_sched_thread(TRUE);
    SPIN_LOCK(obj->spin);

    if ((deadline != 0) && (POK_GETTICK() >= deadline)) {
      pok_lockobj_remove_thread(&obj->fifo, POK_SCHED_CURRENT_THREAD);
      SPIN_UNLOCK(obj->spin);
      return POK_ERRNO_TIMEOUT;
    } else {
      SPIN_UNLOCK(obj->spin);
      return POK_ERRNO_OK;
    }
  }
}

pok_ret_t pok_lockobj_unlock(pok_lockobj_t *obj,
                             const pok_lockobj_lockattr_t *attr) {

  (void)attr; /* unused at this time */

  if (obj->initialized == FALSE) {
    return POK_ERRNO_LOCKOBJ_NOTREADY;
  }
  // Take the lock object internal lock
  SPIN_LOCK(obj->spin);

  /*
   * There exist three scenarios:
   *     - Scenario 1: some resources are available prior to the unlock
   *     - Scenario 2: no resources are available prior to the unlock and
   *                   no thread is waiting for the resource
   *     - Scenario 3: no resources are available prior to the unlock and
   *                   at least one thread is blocked waiting for a resource
   */

  if (obj->current_value) {
    /*
     * Scenario 1: some resources are available priori to the unlock.
     */

    // No thread should be blocked waiting for a resource since it would
    // have obtained it already.
    assert(pok_lockobj_fifo_is_empty(&obj->fifo));

    if (obj->kind == POK_LOCKOBJ_KIND_SEMAPHORE) {
      // If the lock object is a semaphore, increase the number of available
      // resources if it has not yet reached the maximum value.
      if (obj->current_value < obj->max_value) {
        obj->current_value++;
      }
    } else {
      // Otherwise the lock object is limited to one resource. We do not use
      // an assertion here because this is likely a user-code problem, not an
      // internal one.
#if POK_NEEDS_DEBUG
      printf("[KERNEL] [DEBUG] Try to unlock a lock which is already "
             "unlocked\n");
#endif
      obj->current_value = 1;
    }
    // Release the lock object internal lock and return
    SPIN_UNLOCK(obj->spin);
    return POK_ERRNO_OK;
    // End of scenario 1
  }

  if (pok_lockobj_fifo_is_empty(&obj->fifo)) {
    /*
     * Scenario 2: no resources are available prior to the unlock and
     * no thread is waiting for the resource.
     */

    // Increment the number of available resources (0 -> 1). This is
    // always a valid operation.
    obj->current_value = 1;

    // Release the lock object internal lock and return
    SPIN_UNLOCK(obj->spin);
    return 0;
    // End of scenario 2
  }

  /*
   * Scenario 3: no resources are available prior to the unlock and
   * at least one thread is blocked waiting for a resource.
   */

  // Select a thread to unblock and make it ready again since it
  // has now acquired a resource. The number of available resources
  // does not change.
  uint32_t to_unblock = pok_lockobj_get_head(&obj->fifo);
  pok_lockobj_dequeue(&obj->fifo);
  pok_sched_unlock_thread(to_unblock);

  // Release the lock object internal lock, reschedule in case the unblocked
  // thread has a greater priority than the one running on the processor it
  // has an affinity with and return.
  SPIN_UNLOCK(obj->spin);
  if (!IS_LOCK(obj->eventspin)) {
    pok_threads_schedule_one_proc(pok_threads[to_unblock].processor_affinity);
  }
  return POK_ERRNO_OK;
  // End of scenario 3
}

#ifdef POK_NEEDS_LOCKOBJECTS
pok_ret_t pok_lockobj_partition_wrapper(const pok_lockobj_id_t id,
                                        const pok_lockobj_lockattr_t *attr) {
  /* First, we check that the locked object belongs to the partition
   * If not, we return an error
   */
  pok_ret_t ret;

  if (id < pok_partitions[POK_SCHED_CURRENT_PARTITION].lockobj_index_low) {
    return POK_ERRNO_EINVAL;
  }

  if (id >= pok_partitions[POK_SCHED_CURRENT_PARTITION].lockobj_index_high) {
    return POK_ERRNO_EINVAL;
  }

  if (pok_partitions_lockobjs[id].kind != attr->obj_kind) {
    return POK_ERRNO_EINVAL;
  }

  switch (attr->operation) {
  case LOCKOBJ_OPERATION_LOCK:
    ret = pok_lockobj_lock(&pok_partitions_lockobjs[id], attr);
    return ret;

  case LOCKOBJ_OPERATION_UNLOCK: {
    ret = pok_lockobj_unlock(&pok_partitions_lockobjs[id], attr);
    return ret;
  }

  case LOCKOBJ_OPERATION_WAIT: {
    ret = pok_lockobj_eventwait(&pok_partitions_lockobjs[id], attr->timeout);
    return ret;
  }

  case LOCKOBJ_OPERATION_SIGNAL: {
    ret = pok_lockobj_eventsignal(&pok_partitions_lockobjs[id]);
    return ret;
  }

  case LOCKOBJ_OPERATION_BROADCAST: {
    ret = pok_lockobj_eventbroadcast(&pok_partitions_lockobjs[id]);
    return ret;
  }

  default:
    return POK_ERRNO_EINVAL;
  }
}
#endif

void pok_lockobj_fifo_init(pok_lockobj_fifo_t *fifo) {
  fifo->head = fifo->last = 0;
  fifo->is_empty = TRUE;
}
uint32_t pok_lockobj_get_head(pok_lockobj_fifo_t *fifo) {
  return fifo->buffer[fifo->head];
}
pok_ret_t pok_lockobj_enqueue(pok_lockobj_fifo_t *fifo, uint32_t thread,
                              pok_queueing_discipline_t queueing_discipline) {
  if (queueing_discipline == POK_QUEUEING_DISCIPLINE_FIFO) {
    if (!fifo->is_empty && fifo->last == fifo->head)
      return POK_ERRNO_FULL;
    fifo->buffer[fifo->last] = thread;
    fifo->last = (fifo->last + 1) % POK_CONFIG_NB_THREADS;
    fifo->is_empty = FALSE;
    return POK_ERRNO_OK;
  } else if (queueing_discipline == POK_QUEUEING_DISCIPLINE_PRIORITY) {
    uint32_t i = fifo->head;
    while ((i != fifo->last) && (pok_threads[thread].priority <=
                                 pok_threads[fifo->buffer[i]].priority)) {
      i = (i + 1) % POK_CONFIG_NB_THREADS;
    }
    for (uint32_t j = fifo->last; j != i;
         j = (j ? (j - 1) : (POK_CONFIG_NB_THREADS - 1))) {
      fifo->buffer[j] = fifo->buffer[j ? (j - 1) : (POK_CONFIG_NB_THREADS - 1)];
    }
    fifo->buffer[i] = thread;
    fifo->is_empty = FALSE;
    fifo->last = (fifo->last + 1) % POK_CONFIG_NB_THREADS;
    return POK_ERRNO_OK;

  } else {
#if POK_NEEDS_DEBUG
    printf("Wrong policy\n");
#endif
    return POK_ERRNO_PARAM;
  }
}
pok_ret_t pok_lockobj_dequeue(pok_lockobj_fifo_t *fifo) {
  if (fifo->is_empty)
    return POK_ERRNO_EMPTY;
  fifo->head = (fifo->head + 1) % POK_CONFIG_NB_THREADS;
  if (fifo->last == fifo->head)
    fifo->is_empty = TRUE;
  return POK_ERRNO_OK;
}

bool_t pok_lockobj_fifo_is_empty(pok_lockobj_fifo_t *fifo) {
  return fifo->is_empty;
}

pok_ret_t pok_lockobj_remove_thread(pok_lockobj_fifo_t *fifo, uint32_t thread) {
  if (fifo->is_empty)
    return POK_ERRNO_NOTFOUND;
  uint32_t tmp = POK_CONFIG_NB_THREADS;
  uint32_t i = fifo->head;
  do {
    if (fifo->buffer[i] == thread) {
      tmp = i;
      break;
    }
    i = (i + 1) % POK_CONFIG_NB_THREADS;
  } while (i != fifo->last);
  if (tmp == POK_CONFIG_NB_THREADS)
    return POK_ERRNO_NOTFOUND;
  fifo->last = (fifo->last ? (fifo->last - 1) : (POK_CONFIG_NB_THREADS - 1));
  while (tmp != fifo->last) {
    fifo->buffer[tmp] = fifo->buffer[(tmp + 1) % POK_CONFIG_NB_THREADS];
    tmp = (tmp + 1) % POK_CONFIG_NB_THREADS;
  }
  if (fifo->last == fifo->head)
    fifo->is_empty = TRUE;
  return POK_ERRNO_OK;
}

#endif
