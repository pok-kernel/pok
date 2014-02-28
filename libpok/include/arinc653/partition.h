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


#ifdef POK_NEEDS_ARINC653_PARTITION

#include <arinc653/types.h>
#include <arinc653/process.h>

#ifndef APEX_PARTITION
#define APEX_PARTITION
#define   MAX_NUMBER_OF_PARTITIONS   SYSTEM_LIMIT_NUMBER_OF_PARTITIONS
typedef enum
{
   IDLE       = 0,
   COLD_START = 1,
   WARM_START = 2,
   NORMAL     = 3
} OPERATING_MODE_TYPE;

typedef   APEX_INTEGER     PARTITION_ID_TYPE;
typedef enum
{
   NORMAL_START          = 0,
   PARTITION_RESTART     = 1,
   HM_MODULE_RESTART     = 2,
   HM_PARTITION_RESTART  = 3
} START_CONDITION_TYPE;

typedef struct {
   SYSTEM_TIME_TYPE      PERIOD;
   SYSTEM_TIME_TYPE      DURATION;
   PARTITION_ID_TYPE     IDENTIFIER;
   LOCK_LEVEL_TYPE       LOCK_LEVEL;
   OPERATING_MODE_TYPE OPERATING_MODE;
   START_CONDITION_TYPE START_CONDITION;
} PARTITION_STATUS_TYPE;

extern void GET_PARTITION_STATUS (
      /*out*/ PARTITION_STATUS_TYPE     *PARTITION_STATUS,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE );
extern void SET_PARTITION_MODE (
      /*in */ OPERATING_MODE_TYPE       OPERATING_MODE,
      /*out*/ RETURN_CODE_TYPE          *RETURN_CODE );
#endif

#endif
