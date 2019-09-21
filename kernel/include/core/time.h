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


#if defined (POK_NEEDS_TIME) || defined (POK_NEEDS_THREADS) || defined (POK_NEEDS_SCHED)

#ifndef __POK_TIME_H__
#define __POK_TIME_H__

#include <types.h>
#include <errno.h>

/**
 * The rate of the clock in POK
 */
#define POK_TIMER_FREQUENCY 100000

/**
 * POK timer quantum
 */
#define POK_TIMER_QUANTUM 1000

extern uint64_t pok_tick_counter;

#define CLOCK_HANDLER pok_tick_counter += 10000; pok_sched (); // 10000 because freq (POK_TIMER_FREQUENCY) is 10e5 and time (pok_tick_counter) is in nanoseconds: 10e9/POK_TIMER_FREQUENCY

#define POK_GETTICK() pok_tick_counter

typedef int clockid_t;

# define CLOCK_REALTIME 0

typedef long int time_t;

struct timespec
{
    time_t tv_sec;              /* Seconds.  */
    long int tv_nsec;           /* Nanoseconds.  */
};

typedef enum
{
   __POK_CLOCK_REALTIME = 0 
} pok_clockid_t;

void        pok_time_init (void);
pok_ret_t   pok_clock_gettime (pok_clockid_t clk_id, struct timespec *tp);
uint64_t    pok_gettick (void);
pok_ret_t   pok_gettick_by_pointer (uint64_t* clk_val);

#endif  /* __POK_TIME_H__ */
#endif /* Needs clauses */
