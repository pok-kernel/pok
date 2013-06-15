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


#ifdef POK_NEEDS_ARINC653_BUFFER


/*----------------------------------------------------------------*/ 
/*                                                                */ 
/* BUFFER constant and type definitions and management services   */ 
/*                                                                */ 
/*----------------------------------------------------------------*/ 
 
#ifndef APEX_BUFFER 
#define APEX_BUFFER 

#ifndef POK_NEEDS_ARINC653_PROCESS
#define POK_NEEDS_ARINC653_PROCESS
#endif

#include <arinc653/types.h>
#include <arinc653/process.h>
 
#define  MAX_NUMBER_OF_BUFFERS    SYSTEM_LIMIT_NUMBER_OF_BUFFERS 
 
typedef  NAME_TYPE       BUFFER_NAME_TYPE; 
 
typedef  APEX_INTEGER    BUFFER_ID_TYPE; 
 
typedef struct { 
   MESSAGE_RANGE_TYPE  NB_MESSAGE; 
   MESSAGE_RANGE_TYPE  MAX_NB_MESSAGE; 
   MESSAGE_SIZE_TYPE   MAX_MESSAGE_SIZE;
   WAITING_RANGE_TYPE  WAITING_PROCESSES;
} BUFFER_STATUS_TYPE; 
 
 
 
extern void CREATE_BUFFER ( 
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME, 
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE, 
       /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE, 
       /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE, 
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void SEND_BUFFER ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */ 
       /*in */ MESSAGE_SIZE_TYPE        LENGTH, 
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void RECEIVE_BUFFER ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT, 
       /*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_BUFFER_ID ( 
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME, 
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
extern void GET_BUFFER_STATUS ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*out*/ BUFFER_STATUS_TYPE       *BUFFER_STATUS, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE ); 
 
#endif 
#endif
