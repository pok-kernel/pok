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

#ifdef POK_NEEDS_ARINC653_TIME

#include <arinc653/time.h>
#include <arinc653/types.h>

#include <core/thread.h>
#include <core/time.h>
#include <types.h>

unsigned long long __udivdi3(unsigned long long num, unsigned long long den);

void TIMED_WAIT(SYSTEM_TIME_TYPE delay_time, RETURN_CODE_TYPE *return_code) {
  *return_code = pok_thread_sleep((pok_time_t)delay_time / 1000);
}

void PERIODIC_WAIT(RETURN_CODE_TYPE *return_code) {
  pok_ret_t core_ret;
  core_ret = pok_thread_period();
  *return_code = core_ret;
}

void GET_TIME(SYSTEM_TIME_TYPE *system_time, RETURN_CODE_TYPE *return_code) {
  *return_code = pok_time_get((pok_time_t *)system_time);
}

#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE
void REPLENISH(SYSTEM_TIME_TYPE budget_time, RETURN_CODE_TYPE *return_code) {
  (void)budget_time;
  *return_code = NOT_AVAILABLE;
}
#endif

#endif
