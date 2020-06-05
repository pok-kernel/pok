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

#ifndef __POK_TIME_H__
#define __POK_TIME_H__

#include <errno.h>
#include <types.h>

/**
 * The rate of the clock change in POK in Hz.
 */
#define POK_TIMER_FREQUENCY 100000

/**
 * POK timer quantum in Hz. Delays and scheduling will be restricted to this
 * precision.
 */
#define POK_TIMER_QUANTUM 1000

extern volatile uint64_t pok_tick_counter;

#define POK_GETTICK() pok_tick_counter

typedef int clockid_t;

#define CLOCK_REALTIME 0

typedef long int time_t;

struct timespec {
  time_t tv_sec;    /* Seconds.  */
  long int tv_nsec; /* Nanoseconds.  */
};

typedef enum { __POK_CLOCK_REALTIME = 0 } pok_clockid_t;

void pok_time_init(void);
pok_ret_t pok_clock_gettime(pok_clockid_t clk_id, struct timespec *tp);
uint64_t pok_gettick(void);

#endif /* __POK_TIME_H__ */
