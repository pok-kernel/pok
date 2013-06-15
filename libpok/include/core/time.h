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


#ifndef __POK_TIME_H__
#define __POK_TIME_H__

#include <core/dependencies.h>

#include <types.h>
#include <errno.h>
#include <core/syscall.h>

#define pok_time_get(v) pok_time_gettick(v)
/*
 * Get the amount of milliseconds expired since the system starts.
 */

#define pok_time_milliseconds(value) value
/*
 * Build a time according to the amount of time given in the first
 * parameter
 * Similar to: pok_time_t pok_time_milliseconds (const uint32_t ms);
 */

#define pok_time_seconds(s) s*1000;
/*
 * Build a time value according to the time given in the first
 * parameter
 * Could be considered as: pok_time_t pok_time_seconds (const uint32_t s);
 */

pok_ret_t pok_time_compute_deadline (const pok_time_t relative, pok_time_t* absolute);
/*
 * Compute a deadline from now according to the first parameter.
 * This function is a facility to compute tasks' deadline.
 */

#define pok_time_gettick(value) pok_syscall2(POK_SYSCALL_GETTICK,(uint32_t)value,0);
/*
 * Get number of ticks that passed since the system starts
 * Similar to : pok_ret_t   pok_time_gettick (uint64_t* value);
 */

#endif  /* __POK_TIME_H__ */
