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

#ifndef __POK_SCHED_H__
#define __POK_SCHED_H__

#include <core/multiprocessing.h>
#include <core/schedvalues.h>
#include <errno.h>
#include <types.h>

extern uint8_t pok_current_partition;
#define POK_SCHED_CURRENT_PARTITION pok_current_partition

extern uint32_t current_threads[POK_CONFIG_NB_PROCESSORS];
#define POK_SCHED_CURRENT_THREAD current_threads[pok_get_proc_id()]

typedef enum {
  POK_STATE_STOPPED = 0,
  POK_STATE_RUNNABLE = 1,
  POK_STATE_WAITING = 2,
  POK_STATE_LOCK = 3,
  POK_STATE_WAIT_NEXT_ACTIVATION = 4,
  POK_STATE_DELAYED_START = 5
} pok_state_t;

void pok_sched_init(void); /* Initialize scheduling stuff */

void pok_global_sched(void); /* Main scheduling function, this function
                              * schedules everything
                              */
void pok_global_sched_thread(bool_t is_source_processor);
void pok_sched_thread(bool_t is_source_processor);
void pok_threads_schedule_one_proc(uint8_t dest);
void pok_threads_schedule_every_proc(void);

/* Get priority function, return are made according to a scheduler */
uint8_t pok_sched_get_priority_min(const pok_sched_t sched_type);
uint8_t pok_sched_get_priority_max(const pok_sched_t sched_type);

/* Scheduler election method */
uint8_t pok_sched_election(void);
uint32_t pok_sched_part_rr(const uint32_t, const uint32_t,
                           const uint32_t prev_thread,
                           const uint32_t current_thread);
uint32_t pok_sched_part_rms(const uint32_t, const uint32_t,
                            const uint32_t prev_thread,
                            const uint32_t current_thread);
uint32_t pok_sched_part_static(const uint32_t, const uint32_t,
                               const uint32_t prev_thread,
                               const uint32_t current_thread);

/* Context switch functions */
void pok_global_sched_context_switch(const uint32_t elected_id,
                                     bool_t is_source_processor);
void pok_sched_context_switch(const uint32_t elected_id,
                              bool_t is_source_processor);
void pok_partition_switch(void);

/*
 * Functions to lock threads
 */
void pok_sched_lock_thread(const uint32_t);
void pok_sched_unlock_thread(const uint32_t);
void pok_sched_lock_current_thread(void);
void pok_sched_lock_current_thread_timed(const uint64_t time);
void pok_sched_stop_thread(const uint32_t tid);
void pok_sched_stop_self(void);
pok_ret_t pok_sched_end_period();
void pok_sched_activate_error_thread(void);

uint32_t pok_sched_get_current(uint32_t *thread_id);

#endif /* !__POK_SCHED_H__ */
