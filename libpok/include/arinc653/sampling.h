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


#ifdef POK_NEEDS_ARINC653_SAMPLING

#include <arinc653/types.h>

/*--------------------------------------------------------------------*/ 
/*                                                                    */ 
/* SAMPLING PORT constant and type definitions and management services*/ 
/*                                                                    */ 
/*--------------------------------------------------------------------*/ 
 
#ifndef APEX_SAMPLING 
#define APEX_SAMPLING 
 
#define  MAX_NUMBER_OF_SAMPLING_PORTS   SYSTEM_LIMIT_NUMBER_OF_SAMPLING_PORTS 
 
typedef  NAME_TYPE       SAMPLING_PORT_NAME_TYPE; 
 
typedef  APEX_INTEGER    SAMPLING_PORT_ID_TYPE; 
 
typedef  enum { INVALID = 0, VALID = 1 } VALIDITY_TYPE; 
 
typedef struct
{ 
   SYSTEM_TIME_TYPE         REFRESH_PERIOD; 
   MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE; 
   PORT_DIRECTION_TYPE      PORT_DIRECTION; 
   VALIDITY_TYPE            LAST_MSG_VALIDITY; 
} SAMPLING_PORT_STATUS_TYPE; 
 
extern void CREATE_SAMPLING_PORT ( 
       /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME, 
       /*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE, 
       /*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION, 
       /*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD, 
       /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID, 
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE ); 
 
extern void WRITE_SAMPLING_MESSAGE ( 
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID, 
       /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,     /* by reference */ 
       /*in */ MESSAGE_SIZE_TYPE          LENGTH, 
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE ); 
 
extern void READ_SAMPLING_MESSAGE ( 
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID, 
       /*out*/ MESSAGE_ADDR_TYPE          MESSAGE_ADDR, 
       /*out*/ MESSAGE_SIZE_TYPE          *LENGTH, 
       /*out*/ VALIDITY_TYPE              *VALIDITY, 
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE ); 
 
extern void GET_SAMPLING_PORT_ID ( 
       /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME, 
       /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID, 
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE ); 
 
extern void GET_SAMPLING_PORT_STATUS ( 
       /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID, 
       /*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS, 
       /*out*/ RETURN_CODE_TYPE           *RETURN_CODE ); 
 
#endif 

#endif
