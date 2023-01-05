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
 *                                      Copyright (c) 2007-2022 POK team
 */

/**
 * \file    core/thread.c
 * \author  Julien Delange
 * \date    2008-2009
 * \brief   Thread management in kernel
 */

#include <types.h>

#include <arch.h>
#include <assert.h>
#include <core/debug.h>
#include <core/error.h>
#include <core/multiprocessing.h>
#include <core/partition.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/time.h>

#include <core/instrumentation.h>

/**
 * We declare an array of threads. The amount of threads
 * is fixed by the software developper and we add two theads
 *    - one for the kernel thread (this code)
 *    - one for the idle task
 */
pok_thread_t pok_threads[POK_CONFIG_NB_THREADS];

extern uint64_t partition_processor_affinity[];

extern pok_partition_t pok_partitions[POK_CONFIG_NB_PARTITIONS];

#ifdef POK_NEEDS_SCHED_RMS

/**
 * This part of the code sort the threads according to their
 * periods. This part is dedicated to the RMS scheduling algorithm.
 */
void pok_thread_insert_sort(uint16_t index_low, uint16_t index_high) {
  uint32_t i = index_high, j = 0;
  pok_thread_t val;

  val = pok_threads[i];
  j = i - 1;
  while (j >= index_low && pok_threads[j].period > val.period) {
    pok_threads[j + 1] = pok_threads[j];
    j--;
  }
  pok_threads[j + 1] = val;
}
#endif

void pok_idle_thread_init() {

  for (int i = 0; i < POK_CONFIG_NB_PROCESSORS; i++) {

    pok_threads[IDLE_THREAD - i].period = INFINITE_TIME_VALUE;
    pok_threads[IDLE_THREAD - i].deadline = 100000;
    pok_threads[IDLE_THREAD - i].ab_deadline = 100000;
    pok_threads[IDLE_THREAD - i].time_capacity = INFINITE_TIME_VALUE;
    pok_threads[IDLE_THREAD - i].next_activation = 0;
    pok_threads[IDLE_THREAD - i].remaining_time_capacity = 100000;
    pok_threads[IDLE_THREAD - i].wakeup_time = 0;
    pok_threads[IDLE_THREAD - i].entry = pok_arch_idle;
    pok_threads[IDLE_THREAD - i].processor_affinity = i;
    pok_threads[IDLE_THREAD - i].base_priority = pok_sched_get_priority_min(0);
    pok_threads[IDLE_THREAD - i].state = POK_STATE_RUNNABLE;

    pok_threads[IDLE_THREAD - i].sp = pok_context_create(
        IDLE_THREAD - i, IDLE_STACK_SIZE, (uint32_t)pok_arch_idle);
  }
}

/**
 * Initialize threads array, put their default values
 * and so on
 */
void pok_thread_init(void) {
  uint32_t i;

  uint32_t total_threads;
  uint8_t j;

  total_threads = 0;

  for (j = 0; j < POK_CONFIG_NB_PARTITIONS; j++) {
    total_threads = total_threads + pok_partitions[j].nthreads;
  }

  if (total_threads != (POK_CONFIG_NB_THREADS - 1 - POK_CONFIG_NB_PROCESSORS)) {
#ifdef POK_NEEDS_DEBUG
    printf("Error in configuration, bad number of threads\n");
#endif
    pok_kernel_error(POK_ERROR_KIND_KERNEL_CONFIG);
  }
  for (i = 0; i < POK_CONFIG_NB_THREADS; ++i) {
    pok_threads[i].period = INFINITE_TIME_VALUE;
    pok_threads[i].deadline = 0;
    pok_threads[i].ab_deadline = 0;
    pok_threads[i].time_capacity = INFINITE_TIME_VALUE;
    pok_threads[i].remaining_time_capacity = -1;
    pok_threads[i].next_activation = 0;
    pok_threads[i].wakeup_time = 0;
    pok_threads[i].state = POK_STATE_STOPPED;
    pok_threads[i].processor_affinity = 0;
    pok_threads[i].weight = 1;
    pok_threads[i].remaining_round = POK_LAB_SCHED_ROUND;
  }
  pok_idle_thread_init();
}

/**
 * Create a thread inside a partition
 * Return POK_ERRNO_OK if no error.
 * Return POK_ERRNO_TOOMANY if the partition cannot contain
 * more threads.
 */
pok_ret_t pok_partition_thread_create(uint32_t *thread_id,
                                      const pok_thread_attr_t *attr,
                                      const uint8_t partition_id) {
  uint32_t stack_vaddr;
  /**
   * We can create a thread only if the partition is in INIT mode
   */
  if ((pok_partitions[partition_id].mode != POK_PARTITION_MODE_INIT_COLD) &&
      (pok_partitions[partition_id].mode != POK_PARTITION_MODE_INIT_WARM)) {
    return POK_ERRNO_MODE;
  }

  // TODO: this looks suspicious
  uint32_t id = pok_partitions[partition_id].thread_index_low +
                pok_partitions[partition_id].thread_index;
  if (id >= pok_partitions[partition_id].thread_index_high) {
    POK_ERROR_CURRENT_PARTITION(POK_ERROR_KIND_PARTITION_CONFIGURATION);
    return POK_ERRNO_TOOMANY;
  }

  pok_partitions[partition_id].thread_index =
      pok_partitions[partition_id].thread_index + 1;
  assert(id >= pok_partitions[partition_id].thread_index_low);
  assert(id < pok_partitions[partition_id].thread_index_high);

  if ((attr->priority <=
       pok_sched_get_priority_max(pok_partitions[partition_id].sched)) &&
      (attr->priority >=
       pok_sched_get_priority_min(pok_partitions[partition_id].sched))) {
    pok_threads[id].priority = attr->priority;
    pok_threads[id].base_priority = attr->priority;
  }

  if (attr->period > 0) {
    pok_threads[id].period = attr->period;
    pok_threads[id].next_activation = attr->period;
    pok_threads[id].deadline = attr->period;
  }

  if (attr->weight > 0){
    pok_threads[id].weight = attr->weight;
  }

  if (attr->deadline > 0) {
    pok_threads[id].ab_deadline = attr->deadline;
    pok_threads[id].deadline = attr->deadline;
  }

  if (attr->time_capacity > 0) {
    pok_threads[id].time_capacity = attr->time_capacity;
    pok_threads[id].remaining_time_capacity = attr->time_capacity;
  } else {
    pok_threads[id].remaining_time_capacity = POK_THREAD_DEFAULT_TIME_CAPACITY;
    pok_threads[id].time_capacity = POK_THREAD_DEFAULT_TIME_CAPACITY;
  }

  pok_threads[id].processor_affinity =
      get_proc_real_id(partition_id, attr->processor_affinity);

  pok_threads[id].remaining_round =POK_LAB_SCHED_ROUND;

  assert(multiprocessing_system
             ? pok_threads[id].processor_affinity < multiprocessing_system
             : pok_threads[id].processor_affinity == 0);
  assert((1 << pok_threads[id].processor_affinity) &
         partition_processor_affinity[partition_id]);
  assert(
      (pok_partitions[partition_id].thread_main_entry == (uint32_t)attr->entry)
          ? pok_threads[id].processor_affinity ==
                pok_partitions[partition_id].thread_main_proc
          : 1);

  stack_vaddr = pok_thread_stack_addr(
      partition_id, pok_partitions[partition_id].thread_index);

  pok_threads[id].state = POK_STATE_RUNNABLE;
  pok_threads[id].wakeup_time = 0;
  pok_threads[id].ab_deadline = pok_threads[id].wakeup_time + pok_threads[id].deadline;
  pok_threads[id].sp = pok_space_context_create(
      partition_id, (uint32_t)attr->entry, pok_threads[id].processor_affinity,
      stack_vaddr, 0xdead, 0xbeaf);
  /*
   *  FIXME : current debug session about exceptions-handled
  printf ("thread sp=0x%x\n", pok_threads[id].sp);
  printf ("thread stack vaddr=0x%x\n", stack_vaddr);
  */
  pok_threads[id].partition = partition_id;
  pok_threads[id].entry = attr->entry;
  pok_threads[id].init_stack_addr = stack_vaddr;
  *thread_id = id;

#ifdef POK_NEEDS_SCHED_RMS
  if ((pok_partitions[partition_id].sched == POK_SCHED_RMS) &&
      (id > pok_partitions[partition_id].thread_index_low)) {
    pok_thread_insert_sort(pok_partitions[partition_id].thread_index_low + 1,
                           id);
  }
#endif

#ifdef POK_NEEDS_INSTRUMENTATION
  pok_instrumentation_task_archi(id);
#endif

  return POK_ERRNO_OK;
}

/**
 * Start a thread, giving its entry call with \a entry
 * and its identifier with \a id
 */
void pok_thread_start(void (*entry)(), unsigned int id) {
  (void)id;
  entry();
}

#ifdef POK_NEEDS_THREAD_SLEEP
pok_ret_t pok_thread_sleep(uint32_t us) {
  uint64_t mytime;
  mytime = ((uint64_t)us) * 1000 + POK_GETTICK();
  pok_sched_lock_current_thread_timed(mytime);
  pok_sched_thread(TRUE);
  return POK_ERRNO_OK;
}
#endif

#ifdef POK_NEEDS_THREAD_SLEEP_UNTIL
pok_ret_t pok_thread_sleep_until(uint32_t us) {
  pok_sched_lock_current_thread_timed(((uint64_t)us) * 1000);
  pok_sched_thread(TRUE);
  return POK_ERRNO_OK;
}
#endif

pok_ret_t pok_thread_suspend(void) {
  pok_sched_stop_self();
  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_restart(const uint32_t tid) {
  /**
   * Reinit timing values
   */

  pok_threads[tid].remaining_time_capacity = pok_threads[tid].time_capacity;
  pok_threads[tid].state = POK_STATE_WAIT_NEXT_ACTIVATION;
  pok_threads[tid].wakeup_time = 0;

  /**
   * Newer solution for later improvements.
   *
   pok_space_context_restart (pok_threads[tid].sp, (uint32_t)
   pok_threads[tid].entry, pok_threads[tid].init_stack_addr);
   *
   */

  /**
   * At this time, we build a new context for the thread.
   * It is not the best solution but it works at this time
   */
  pok_threads[tid].sp = pok_space_context_create(
      pok_threads[tid].partition, (uint32_t)pok_threads[tid].entry,
      pok_threads[tid].processor_affinity, pok_threads[tid].init_stack_addr,
      0xdead, 0xbeaf);

  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_delayed_start(const uint32_t id, const uint32_t us) {
  uint64_t ns = 1000 * ((uint64_t)us);
  if (POK_CURRENT_PARTITION.thread_index_low > id ||
      POK_CURRENT_PARTITION.thread_index_high < id)
    return POK_ERRNO_THREADATTR;
  pok_threads[id].priority = pok_threads[id].base_priority;
  // reset stack
  pok_context_reset(POK_USER_STACK_SIZE, pok_threads[id].init_stack_addr);
  if ((long long)pok_threads[id].period == INFINITE_TIME_VALUE) {
    if (pok_partitions[pok_threads[id].partition].mode ==
        POK_PARTITION_MODE_NORMAL) {
      if (ns == 0) {
        pok_threads[id].state = POK_STATE_RUNNABLE;
        if (pok_threads[id].time_capacity > 0)
          pok_threads[id].end_time =
              POK_GETTICK() + pok_threads[id].time_capacity;
      } else {
        pok_threads[id].state = POK_STATE_WAITING;
        pok_threads[id].wakeup_time = POK_GETTICK() + ns;
      }
      // the preemption is always enabled so
      pok_global_sched();
    } else // the partition mode is cold or warm start
    {
      pok_threads[id].state = POK_STATE_DELAYED_START;
      pok_threads[id].wakeup_time = ns;
    }
  } else {
    if (pok_partitions[pok_threads[id].partition].mode ==
        POK_PARTITION_MODE_NORMAL) { // set the first release point
      pok_threads[id].next_activation = ns + POK_GETTICK();
      pok_threads[id].end_time =
          pok_threads[id].deadline + pok_threads[id].next_activation;
    } else {
      pok_threads[id].state = POK_STATE_DELAYED_START;
      pok_threads[id].wakeup_time =
          ns; // temporarly storing the delay, see set_partition_mode
    }
  }
  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_get_status(const uint32_t id, pok_thread_attr_t *attr) {
  if (POK_CURRENT_PARTITION.thread_index_low > id ||
      POK_CURRENT_PARTITION.thread_index_high < id)
    return POK_ERRNO_PARAM;
  attr->deadline = pok_threads[id].end_time;
  attr->state = pok_threads[id].state;
  attr->priority = pok_threads[id].priority;
  attr->entry = pok_threads[id].entry;
  attr->period = pok_threads[id].period;
  attr->time_capacity = pok_threads[id].time_capacity;
  attr->stack_size = POK_USER_STACK_SIZE;
  attr->processor_affinity = get_proc_partition_id(
      POK_SCHED_CURRENT_PARTITION, pok_threads[id].processor_affinity);
  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_set_priority(const uint32_t id, const uint32_t priority) {
  if (POK_CURRENT_PARTITION.thread_index_low > id ||
      POK_CURRENT_PARTITION.thread_index_high < id)
    return POK_ERRNO_PARAM;
  pok_threads[id].priority = priority;
  /* preemption is always enabled so ... */
  pok_global_sched();
  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_resume(const uint32_t id) {
  if (POK_CURRENT_PARTITION.thread_index_low > id ||
      POK_CURRENT_PARTITION.thread_index_high < id)
    return POK_ERRNO_THREADATTR;
  pok_threads[id].wakeup_time = POK_GETTICK();
  pok_threads[id].state = POK_STATE_RUNNABLE;
  /* preemption is always enabled */
  pok_global_sched();
  return POK_ERRNO_OK;
}

pok_ret_t pok_thread_suspend_target(const uint32_t id) {
  if (POK_CURRENT_PARTITION.thread_index_low > id ||
      POK_CURRENT_PARTITION.thread_index_high < id ||
      id == POK_SCHED_CURRENT_THREAD)
    return POK_ERRNO_THREADATTR;
  pok_threads[id].state = POK_STATE_STOPPED;
  return POK_ERRNO_OK;
}
