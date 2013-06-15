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


#ifdef POK_NEEDS_ARINC653_ERROR
#ifndef APEX_ERROR
#define APEX_ERROR

#ifndef POK_NEEDS_ARINC653_PROCESS
#define POK_NEEDS_ARINC653_PROCESS 1
#endif

#include <arinc653/process.h>

#include <arinc653/types.h>

#define  MAX_ERROR_MESSAGE_SIZE         64 
 
typedef  APEX_INTEGER   ERROR_MESSAGE_SIZE_TYPE; 
 
typedef  APEX_BYTE      ERROR_MESSAGE_TYPE[MAX_ERROR_MESSAGE_SIZE]; 
 
enum ERROR_CODE_VALUE_TYPE {
   DEADLINE_MISSED      = 0,
   APPLICATION_ERROR    = 1,
   NUMERIC_ERROR        = 2,
   ILLEGAL_REQUEST      = 3,
   STACK_OVERFLOW       = 4,
   MEMORY_VIOLATION     = 5,
   HARDWARE_FAULT       = 6,
   POWER_FAIL           = 7
};

typedef enum ERROR_CODE_VALUE_TYPE ERROR_CODE_TYPE;
/*------------------------------*/
/* error status type */
/*------------------------------*/
typedef struct{
   ERROR_CODE_TYPE         ERROR_CODE;
   MESSAGE_SIZE_TYPE       LENGTH;
   PROCESS_ID_TYPE         FAILED_PROCESS_ID;
   SYSTEM_ADDRESS_TYPE     FAILED_ADDRESS;
   ERROR_MESSAGE_TYPE      MESSAGE;
} ERROR_STATUS_TYPE;

/*----------------------------------------------------------------------*/
/* */
/* ERROR MANAGEMENT SERVICES */
/* */
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

extern void REPORT_APPLICATION_MESSAGE (MESSAGE_ADDR_TYPE    MESSAGE,
                                        MESSAGE_SIZE_TYPE    LENGTH,
                                        RETURN_CODE_TYPE     *RETURN_CODE);

extern void CREATE_ERROR_HANDLER (SYSTEM_ADDRESS_TYPE  ENTRY_POINT,
                                  STACK_SIZE_TYPE      STACK_SIZE,
                                  RETURN_CODE_TYPE     *RETURN_CODE);

extern void GET_ERROR_STATUS (ERROR_STATUS_TYPE  *ERROR_STATUS,
                              RETURN_CODE_TYPE   *RETURN_CODE );

extern void RAISE_APPLICATION_ERROR (ERROR_CODE_TYPE            ERROR_CODE,
                                     MESSAGE_ADDR_TYPE          MESSAGE,
                                     ERROR_MESSAGE_SIZE_TYPE    LENGTH,
                                     RETURN_CODE_TYPE           *RETURN_CODE);
#endif
#endif 
