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
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifndef __POK_TIME_H__
#define __POK_TIME_H__

#include <core/dependencies.h>

#include <core/syscall.h>
#include <errno.h>
#include <types.h>

pok_ret_t pok_time_get(pok_time_t *ns);
/*
 * Get the amount of nanoseconds expired since the system starts.
 */

#define pok_time_milliseconds(value) value
/*
 * Build a time according to the amount of time given in the first
 * parameter
 * Similar to: pok_time_t pok_time_milliseconds (const uint32_t ms);
 */

#define pok_time_seconds(s) s * 1000;
/*
 * Build a time value according to the time given in the first
 * parameter
 * Could be considered as: pok_time_t pok_time_seconds (const uint32_t s);
 */

pok_ret_t pok_time_compute_deadline(const pok_time_t relative,
                                    pok_time_t *absolute);
/*
 * Compute a deadline from now according to the first parameter.
 * This function is a facility to compute tasks' deadline.
 */

pok_ret_t pok_time_gettick(pok_time_t *value);

#endif /* __POK_TIME_H__ */
