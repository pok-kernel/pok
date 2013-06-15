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
#include <arinc653/types.h>
#include <arinc653/buffer.h>
#include <types.h>
#include <middleware/port.h>
#include <middleware/buffer.h>
 
void CREATE_BUFFER ( 
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME, 
       /*in */ MESSAGE_SIZE_TYPE        MAX_MESSAGE_SIZE, 
       /*in */ MESSAGE_RANGE_TYPE       MAX_NB_MESSAGE, 
       /*in */ QUEUING_DISCIPLINE_TYPE  QUEUING_DISCIPLINE, 
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_ret_t                  core_ret;
   pok_buffer_id_t            core_id;
   pok_queueing_discipline_t  core_discipline;

   switch (QUEUING_DISCIPLINE)
   {
     case PRIORITY:
         core_discipline = POK_PORT_QUEUEING_DISCIPLINE_PRIORITY;
         break;

     case FIFO:
         core_discipline = POK_PORT_QUEUEING_DISCIPLINE_FIFO;
         break;

      default:
         *RETURN_CODE = INVALID_PARAM;
         return;
   }

   core_ret = pok_buffer_create (BUFFER_NAME, MAX_MESSAGE_SIZE * MAX_NB_MESSAGE, MAX_MESSAGE_SIZE, core_discipline, &core_id);

   *BUFFER_ID = core_id;

   *RETURN_CODE = core_ret;
}
 
void SEND_BUFFER ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*in */ MESSAGE_ADDR_TYPE        MESSAGE_ADDR,       /* by reference */ 
       /*in */ MESSAGE_SIZE_TYPE        LENGTH, 
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_ret_t core_ret;
   core_ret = pok_buffer_send (BUFFER_ID, MESSAGE_ADDR, LENGTH, TIME_OUT);
   *RETURN_CODE = core_ret;
}
 
void RECEIVE_BUFFER ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*in */ SYSTEM_TIME_TYPE         TIME_OUT, 
       /*out*/ MESSAGE_ADDR_TYPE        MESSAGE_ADDR, 
       /*out*/ MESSAGE_SIZE_TYPE        *LENGTH, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   pok_ret_t core_ret;
   pok_port_size_t core_size;
   core_ret = pok_buffer_receive (BUFFER_ID, TIME_OUT, MESSAGE_ADDR, &core_size);
   *LENGTH = (APEX_INTEGER) core_size;
   *RETURN_CODE = core_ret;
}
 
void GET_BUFFER_ID ( 
       /*in */ BUFFER_NAME_TYPE         BUFFER_NAME, 
       /*out*/ BUFFER_ID_TYPE           *BUFFER_ID, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   (void) BUFFER_NAME;
   (void) BUFFER_ID;
   *RETURN_CODE = NOT_AVAILABLE;
}
 
void GET_BUFFER_STATUS ( 
       /*in */ BUFFER_ID_TYPE           BUFFER_ID, 
       /*out*/ BUFFER_STATUS_TYPE       *BUFFER_STATUS, 
       /*out*/ RETURN_CODE_TYPE         *RETURN_CODE )
{
   (void) BUFFER_ID;
   (void) BUFFER_STATUS;
   *RETURN_CODE = NOT_AVAILABLE;
}
 
#endif 
