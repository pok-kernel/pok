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

#include <core/dependencies.h>

#ifdef POK_NEEDS_ARINC653_QUEUEING

#include <types.h>
#include <middleware/port.h>
#include <arinc653/types.h>
#include <arinc653/queueing.h>

void CREATE_QUEUING_PORT (
      /*in */ QUEUING_PORT_NAME_TYPE    QUEUING_PORT_NAME,
      /*in */ MESSAGE_SIZE_TYPE         MAX_MESSAGE_SIZE,
      /*in */ MESSAGE_RANGE_TYPE        MAX_NB_MESSAGE,
      /*in */ PORT_DIRECTION_TYPE       PORT_DIRECTION,
      /*in */ QUEUING_DISCIPLINE_TYPE   QUEUING_DISCIPLINE,
      /*out*/ QUEUING_PORT_ID_TYPE      *QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *return_code)
{
   pok_ret_t                        core_ret;
   pok_port_direction_t             core_direction;
   pok_port_queueing_discipline_t   core_discipline;
   pok_port_id_t                    core_id;

   switch (QUEUING_DISCIPLINE)
   {
     case PRIORITY:
         core_discipline = POK_PORT_QUEUEING_DISCIPLINE_PRIORITY;
         break;

     case FIFO:
         core_discipline = POK_PORT_QUEUEING_DISCIPLINE_FIFO;
         break;

      default:
         *return_code = INVALID_PARAM;
         return;
   }

   switch (PORT_DIRECTION)
   {
      case SOURCE:
         core_direction = POK_PORT_DIRECTION_OUT;
         break;
      
      case DESTINATION:
         core_direction = POK_PORT_DIRECTION_IN;
         break;

      default:
         *return_code = INVALID_PARAM;
         return;
   }

   core_ret = pok_port_queueing_create (QUEUING_PORT_NAME, MAX_MESSAGE_SIZE * MAX_NB_MESSAGE, core_direction, core_discipline, &core_id);

   *QUEUING_PORT_ID = core_id;
   *return_code = core_ret;
}

void SEND_QUEUING_MESSAGE (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*in */ MESSAGE_ADDR_TYPE         MESSAGE_ADDR,       /* by reference */
      /*in */ MESSAGE_SIZE_TYPE         LENGTH,
      /*in */ SYSTEM_TIME_TYPE          TIME_OUT,
      /*out*/ RETURN_CODE_TYPE          *return_code)
{
   pok_ret_t core_ret;

   core_ret = pok_port_queueing_send (QUEUING_PORT_ID, MESSAGE_ADDR, LENGTH, TIME_OUT);

   *return_code = core_ret;
}

void RECEIVE_QUEUING_MESSAGE (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*in */ SYSTEM_TIME_TYPE          TIME_OUT,
      /*out*/ MESSAGE_ADDR_TYPE         MESSAGE_ADDR,
      /*out*/ MESSAGE_SIZE_TYPE         *LENGTH,
      /*out*/ RETURN_CODE_TYPE          *return_code )
{
   pok_ret_t core_ret;
   SYSTEM_TIME_TYPE pok_timeout=TIME_OUT;
   if(TIME_OUT==INFINITE_TIME_VALUE) {
     pok_timeout = 0;
   }
   
   core_ret = pok_port_queueing_receive (QUEUING_PORT_ID, pok_timeout, *LENGTH, MESSAGE_ADDR, (pok_port_size_t*)LENGTH);

   if(core_ret == POK_ERRNO_EMPTY) core_ret = NOT_AVAILABLE;
   *return_code = core_ret;
}

void GET_QUEUING_PORT_ID (
      /*in */ QUEUING_PORT_NAME_TYPE    QUEUING_PORT_NAME,
      /*out*/ QUEUING_PORT_ID_TYPE      *QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *return_code)
{
   pok_ret_t core_ret;

   core_ret = pok_port_queueing_id(QUEUING_PORT_NAME, *QUEUING_PORT_ID);
   *return_code = core_ret;
}

void GET_QUEUING_PORT_STATUS (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*out*/ QUEUING_PORT_STATUS_TYPE *QUEUING_PORT_STATUS,
      /*out*/ RETURN_CODE_TYPE          *return_code)
{
  pok_ret_t core_ret;
  pok_port_queueing_status_t status;

   core_ret = pok_port_queueing_id(QUEUING_PORT_ID, &status);
   QUEUING_PORT_STATUS->NB_MESSAGE = status.nb_messages;
#warning TBSL : commented to have it compile
   //QUEUING_PORT_STATUS->MAX_NB_MESSAGE = status.g;
   QUEUING_PORT_STATUS->MAX_MESSAGE_SIZE = status.size;
   QUEUING_PORT_STATUS->PORT_DIRECTION = status.direction;
   QUEUING_PORT_STATUS->WAITING_PROCESSES = status.waiting_processes;
  *return_code = core_ret;
}

void CLEAR_QUEUING_PORT (
      /*in */ QUEUING_PORT_ID_TYPE      QUEUING_PORT_ID,
      /*out*/ RETURN_CODE_TYPE          *return_code)
{
  (void) QUEUING_PORT_ID;
  *return_code = NOT_AVAILABLE;
}

#endif
