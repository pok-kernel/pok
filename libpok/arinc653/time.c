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


#ifdef POK_NEEDS_ARINC653_TIME

#include <arinc653/types.h>
#include <arinc653/time.h>

#include <core/thread.h>
#include <types.h>


#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE
void TIMED_WAIT (SYSTEM_TIME_TYPE delay_time, RETURN_CODE_TYPE *return_code)
{
   (void) delay_time;
   *return_code = NOT_AVAILABLE;
}
#endif

void PERIODIC_WAIT (RETURN_CODE_TYPE *return_code)
{
   pok_ret_t core_ret;
   core_ret = pok_thread_period ();
   *return_code = core_ret;
}

#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE
void GET_TIME (SYSTEM_TIME_TYPE *system_time, RETURN_CODE_TYPE *return_code)
{
   (void) system_time;
   *return_code = NOT_AVAILABLE;
}

void REPLENISH (SYSTEM_TIME_TYPE budget_time, RETURN_CODE_TYPE *return_code)
{
   (void) budget_time;
   *return_code = NOT_AVAILABLE;
}
#endif

#endif

