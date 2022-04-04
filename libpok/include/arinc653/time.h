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

#ifdef POK_NEEDS_ARINC653_TIME
#ifndef APEX_TIME
#define APEX_TIME

#include <arinc653/types.h>

/*----------------------------------------------------------------------*/
/* */
/* time constant definitions */
/* */
/*----------------------------------------------------------------------*/
/* implementation dependent */
/* these values are given as example */
/*----------------------------------------------------------------------*/
/* */
/* time type definitions */
/* */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* */
/* time management services */
/* */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
extern void TIMED_WAIT(
    /*in */ SYSTEM_TIME_TYPE delay_time,
    /*out*/ RETURN_CODE_TYPE *return_code);
/*----------------------------------------------------------------------*/
extern void PERIODIC_WAIT(
    /*out*/ RETURN_CODE_TYPE *return_code);
/*----------------------------------------------------------------------*/
extern void GET_TIME(
    /*out*/ SYSTEM_TIME_TYPE *system_time,
    /*out*/ RETURN_CODE_TYPE *return_code);
/*----------------------------------------------------------------------*/
void REPLENISH(SYSTEM_TIME_TYPE budget_time, RETURN_CODE_TYPE *return_code);
/*----------------------------------------------------------------------*/
#endif
#endif
