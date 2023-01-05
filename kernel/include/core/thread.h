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

#ifndef __POK_THREAD_H__
#define __POK_THREAD_H__

#include <core/multiprocessing.h>
#include <core/sched.h>
#include <errno.h>
#include <types.h>

/*
 * In POK, we add a kernel thread and an idle thread. The kernel
 * thread is used to execute kernel code while the idle thread
 * is used to save processor resources.
 */

#define KERNEL_THREAD (POK_CONFIG_NB_THREADS - 1)
#define IDLE_THREAD (POK_CONFIG_NB_THREADS - 2 - (uint32_t)(pok_get_proc_id()))

#define INFINITE_TIME_VALUE (-1)
#define POK_THREAD_DEFAULT_TIME_CAPACITY INFINITE_TIME_VALUE

#define POK_THREAD_MAX_PRIORITY 200

/*
 * DEFAULT_STACK_SIZE if the stack size of regulard threads
 * IDLE_STACK_SIZE is the stack size of the idle thread
 */
#define DEFAULT_STACK_SIZE 8192
#define IDLE_STACK_SIZE DEFAULT_STACK_SIZE

#ifndef POK_USER_STACK_SIZE
#define POK_USER_STACK_SIZE DEFAULT_STACK_SIZE
#endif

typedef struct {
  uint8_t priority;
  int64_t period;
  uint64_t deadline;
  int64_t time_capacity;
  int64_t remaining_time_capacity;
  uint64_t next_activation;
  pok_state_t state;
  uint64_t end_time;
  uint64_t wakeup_time;
  void *entry;
  uint8_t partition;
  uint32_t sp;
  uint32_t init_stack_addr;
  uint8_t base_priority;
  uint8_t processor_affinity;
  uint8_t weight;
  uint8_t remaining_round;
  int64_t ab_deadline;
  /* stack pointer
   * FIXME: this is platform-dependent code, we have to handle that ! */
} pok_thread_t;

typedef struct {
  uint8_t priority; /* Priority is from 0 to 255 */
  uint8_t processor_affinity;
  void *entry; /* entrypoint of the thread  */
  uint64_t period;
  uint64_t deadline;
  uint64_t time_capacity;
  uint32_t stack_size;
  pok_state_t state;
  uint8_t weight;
} pok_thread_attr_t;
/*
 * Attributes given to create a thread
 */

void pok_thread_init(void);
void pok_idle_thread_init(void);
pok_ret_t pok_thread_create(uint8_t *thread_id, const pok_thread_attr_t *attr);
pok_ret_t pok_thread_sleep(const uint32_t us);
pok_ret_t pok_thread_sleep_until(const uint32_t us);
pok_ret_t pok_thread_suspend(void);
void pok_thread_start(void (*entry)(), unsigned int id);
pok_ret_t pok_thread_suspend(void);
pok_ret_t pok_thread_restart(const uint32_t tid);
pok_ret_t pok_thread_delayed_start(const uint32_t id, const uint32_t us);
pok_ret_t pok_thread_get_status(const uint32_t id, pok_thread_attr_t *attr);
pok_ret_t pok_thread_set_priority(const uint32_t id, const uint32_t priority);
pok_ret_t pok_thread_resume(const uint32_t id);
pok_ret_t pok_thread_suspend_target(const uint32_t id);
pok_ret_t pok_partition_thread_create(uint32_t *thread_id,
                                      const pok_thread_attr_t *attr,
                                      const uint8_t partition_id);

extern pok_thread_t pok_threads[POK_CONFIG_NB_THREADS];

#define POK_CURRENT_THREAD pok_threads[POK_SCHED_CURRENT_THREAD]

#endif /* __POK_THREAD_H__ */
