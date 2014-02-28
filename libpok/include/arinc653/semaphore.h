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


#ifdef POK_NEEDS_ARINC653_SEMAPHORE
#ifndef APEX_SEMAPHORE
#define APEX_SEMAPHORE

#ifndef POK_NEEDS_ARINC653_PROCESS
#define POK_NEEDS_ARINC653_PROCESS 1
#endif

#include <arinc653/types.h>
#include <arinc653/process.h>

/*----------------------------------------------------------------------*/
/* */
/* SEMAPHORE constant definitions */
/* */
/*----------------------------------------------------------------------*/
/* implementation dependent */
/* these values are given as example */
/*------------------------------*/
/* max nb of semaphores */
/*------------------------------*/
#define MAX_NUMBER_OF_SEMAPHORES 63
/*------------------------------*/
/* max semaphore value */
/*------------------------------*/
#define MAX_SEMAPHORE_VALUE 32767
/*----------------------------------------------------------------------*/
/* */
/* SEMAPHORE type definitions */
/* */
/*----------------------------------------------------------------------*/
/*------------------------------*/
/* semaphore ident type */
/*------------------------------*/
typedef APEX_INTEGER SEMAPHORE_ID_TYPE;
/*------------------------------*/
/* semaphore value type */
/*------------------------------*/
typedef APEX_INTEGER SEMAPHORE_VALUE_TYPE;
/*------------------------------*/
/* semaphore name type */
/*------------------------------*/
typedef NAME_TYPE SEMAPHORE_NAME_TYPE;
/*------------------------------*/
/* semaphore status type */
/*------------------------------*/
typedef
struct {
   SEMAPHORE_VALUE_TYPE CURRENT_VALUE;
   SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE;
   WAITING_RANGE_TYPE WAITING_PROCESSES;
} SEMAPHORE_STATUS_TYPE;
/*----------------------------------------------------------------------*/
/* */
/* semaphore management services */
/* */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
extern void CREATE_SEMAPHORE (
      /*IN */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
      /*IN */ SEMAPHORE_VALUE_TYPE CURRENT_VALUE,
      /*IN */ SEMAPHORE_VALUE_TYPE MAXIMUM_VALUE,
      /*IN */ QUEUING_DISCIPLINE_TYPE QUEUING_DISCIPLINE,
      /*OUT*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
      /*OUT*/ RETURN_CODE_TYPE *RETURN_CODE );
/*----------------------------------------------------------------------*/
extern void WAIT_SEMAPHORE (
      /*IN */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
      /*IN */ SYSTEM_TIME_TYPE TIME_OUT,
      /*OUT*/ RETURN_CODE_TYPE *RETURN_CODE );
/*----------------------------------------------------------------------*/
extern void SIGNAL_SEMAPHORE (
      /*IN */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
      /*OUT*/ RETURN_CODE_TYPE *RETURN_CODE );
/*----------------------------------------------------------------------*/
extern void GET_SEMAPHORE_ID (
      /*IN */ SEMAPHORE_NAME_TYPE SEMAPHORE_NAME,
      /*OUT*/ SEMAPHORE_ID_TYPE *SEMAPHORE_ID,
      /*OUT*/ RETURN_CODE_TYPE *RETURN_CODE );
/*----------------------------------------------------------------------*/
extern void GET_SEMAPHORE_STATUS (
      /*IN */ SEMAPHORE_ID_TYPE SEMAPHORE_ID,
      /*OUT*/ SEMAPHORE_STATUS_TYPE *SEMAPHORE_STATUS,
      /*OUT*/ RETURN_CODE_TYPE *RETURN_CODE );
/*----------------------------------------------------------------------*/
#endif
#endif
