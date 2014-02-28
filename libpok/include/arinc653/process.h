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

#ifdef POK_NEEDS_ARINC653_PROCESS

#include <arinc653/types.h>

#ifndef APEX_PROCESS
#define APEX_PROCESS

#define MAX_NUMBER_OF_PROCESSES  SYSTEM_LIMIT_NUMBER_OF_PROCESSES
#define MIN_PRIORITY_VALUE       1
#define MAX_PRIORITY_VALUE       63
#define MAX_LOCK_LEVEL           16

typedef NAME_TYPE            PROCESS_NAME_TYPE;

typedef APEX_INTEGER         PROCESS_ID_TYPE;

typedef APEX_INTEGER         LOCK_LEVEL_TYPE;

typedef APEX_UNSIGNED        STACK_SIZE_TYPE;

typedef APEX_INTEGER         WAITING_RANGE_TYPE;

typedef APEX_INTEGER         PRIORITY_TYPE;

typedef enum
{
   DORMANT = 0,
   READY    = 1,
   RUNNING = 2,
   WAITING = 3
} PROCESS_STATE_TYPE;

typedef enum
{
   SOFT = 0,
   HARD = 1
} DEADLINE_TYPE;

typedef struct {
   SYSTEM_TIME_TYPE        PERIOD;
   SYSTEM_TIME_TYPE        TIME_CAPACITY;
   SYSTEM_ADDRESS_TYPE     ENTRY_POINT;
   STACK_SIZE_TYPE         STACK_SIZE;
   PRIORITY_TYPE           BASE_PRIORITY;
   DEADLINE_TYPE           DEADLINE;
   PROCESS_NAME_TYPE       NAME;
} PROCESS_ATTRIBUTE_TYPE;

typedef struct {
   SYSTEM_TIME_TYPE          DEADLINE_TIME;
   PRIORITY_TYPE             CURRENT_PRIORITY;
   PROCESS_STATE_TYPE        PROCESS_STATE;
   PROCESS_ATTRIBUTE_TYPE    ATTRIBUTES;
} PROCESS_STATUS_TYPE;

extern void CREATE_PROCESS (
      /*in */ PROCESS_ATTRIBUTE_TYPE     *ATTRIBUTES,
      /*out*/ PROCESS_ID_TYPE            *PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void SET_PRIORITY (
      /*in */ PROCESS_ID_TYPE            PROCESS_ID,
      /*in */ PRIORITY_TYPE              PRIORITY,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void SUSPEND_SELF (
      /*in */ SYSTEM_TIME_TYPE           TIME_OUT,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void SUSPEND (
      /*in */ PROCESS_ID_TYPE            PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void RESUME (
      /*in */ PROCESS_ID_TYPE            PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void STOP_SELF ();

extern void STOP (
      /*in */ PROCESS_ID_TYPE            PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE           *RETURN_CODE );

extern void START (
      /*in */ PROCESS_ID_TYPE     PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void DELAYED_START (
      /*in */ PROCESS_ID_TYPE     PROCESS_ID,
      /*in */ SYSTEM_TIME_TYPE    DELAY_TIME,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void LOCK_PREEMPTION (
      /*out*/ LOCK_LEVEL_TYPE     *LOCK_LEVEL,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void UNLOCK_PREEMPTION (
      /*out*/ LOCK_LEVEL_TYPE     *LOCK_LEVEL,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void GET_MY_ID (
      /*out*/ PROCESS_ID_TYPE     *PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void GET_PROCESS_ID (
      /*in */ PROCESS_NAME_TYPE   PROCESS_NAME[MAX_NAME_LENGTH],
      /*out*/ PROCESS_ID_TYPE     *PROCESS_ID,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

extern void GET_PROCESS_STATUS (
      /*in */ PROCESS_ID_TYPE     PROCESS_ID,
      /*out*/ PROCESS_STATUS_TYPE *PROCESS_STATUS,
      /*out*/ RETURN_CODE_TYPE    *RETURN_CODE );

#endif
#endif
