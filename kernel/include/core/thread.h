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


#ifndef __POK_THREAD_H__
#define __POK_THREAD_H__

#ifdef POK_NEEDS_THREADS

#include <types.h>
#include <errno.h>
#include <core/sched.h>


/*
 * In POK, we add a kernel thread and an idle thread. The kernel
 * thread is used to execute kernel code while the idle thread
 * is used to save processor resources.
 */

#define KERNEL_THREAD		POK_CONFIG_NB_THREADS -2
#define IDLE_THREAD        POK_CONFIG_NB_THREADS -1

#define POK_THREAD_DEFAULT_TIME_CAPACITY 10

/*
#define KERNEL_THREAD		POK_CONFIG_NB_THREADS
#define IDLE_THREAD        POK_CONFIG_NB_THREADS + 1
*/

#define POK_THREAD_MAX_PRIORITY  200


/*
 * IDLE_STACK_SIZE is the stack size of the idle thread
 * DEFAULT_STACK_SIZE if the stack size of regulard threads
 */
#define IDLE_STACK_SIZE		1024
#define DEFAULT_STACK_SIZE	4096

#ifndef POK_USER_STACK_SIZE
#define POK_USER_STACK_SIZE 8096
#endif

typedef struct
{
	 uint8_t      priority;
	 uint64_t     period;
	 uint64_t     deadline;
	 uint64_t     time_capacity;
	 uint64_t     remaining_time_capacity;
	 uint64_t     next_activation;
	 pok_state_t  state;
	 uint64_t	end_time;
	 uint64_t     wakeup_time;
#ifdef POK_NEEDS_SCHED_HFPPS
	 uint64_t	payback; /**< Payback for HFPPS scheduling algorithm */
#endif /* POK_NEEDS_SCHED_HFPPS */
	 void			 *entry;
	 uint8_t		 partition;
	 uint32_t		 sp;
	 uint32_t    init_stack_addr;
  uint8_t	base_priority;
	 /* stack pointer
		* FIXME: this is platform-dependent code, we have to handle that ! */
} pok_thread_t;

typedef struct
{
	 uint8_t      priority;         /* Priority is from 0 to 255 */
	 void*        entry;            /* entrypoint of the thread  */
	 uint64_t     period;
	 uint64_t     deadline;
	 uint64_t     time_capacity;
	 uint32_t     stack_size;
	 pok_state_t  state;
} pok_thread_attr_t;
/*
 * Attributes given to create a thread
 */

void           pok_thread_init (void);
pok_ret_t      pok_thread_create (uint8_t* thread_id, const pok_thread_attr_t* attr);
pok_ret_t      pok_thread_sleep (const uint32_t us);
pok_ret_t      pok_thread_sleep_until (const uint32_t us);
pok_ret_t      pok_thread_suspend (void);
void           pok_thread_start (void (*entry)(), unsigned int id);
pok_ret_t      pok_thread_suspend (void);
pok_ret_t      pok_thread_restart (const uint32_t tid);
pok_ret_t pok_thread_delayed_start (const uint32_t id, const uint32_t us);
pok_ret_t      pok_thread_get_status (const uint32_t id, pok_thread_attr_t *attr);
pok_ret_t      pok_thread_set_priority (const uint32_t id, const uint32_t priority);
pok_ret_t      pok_thread_resume (const uint32_t id);
pok_ret_t      pok_thread_suspend_target (const uint32_t id);

#ifdef POK_NEEDS_PARTITIONS
pok_ret_t		pok_partition_thread_create (uint32_t* thread_id,
																						const pok_thread_attr_t* attr,
																						const uint8_t  partition_id);
#endif

extern pok_thread_t              pok_threads[POK_CONFIG_NB_THREADS];

#define POK_CURRENT_THREAD pok_threads[POK_SCHED_CURRENT_THREAD]

#endif /* __POK_NEEDS_THREADS */

#endif /* __POK_THREAD_H__ */
