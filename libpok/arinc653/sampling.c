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

#include <types.h>
#include <middleware/port.h>
#include <arinc653/types.h>
#include <arinc653/sampling.h>
#include <arinc653/partition.h>
#include <core/thread.h>

void CREATE_SAMPLING_PORT (
			 /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
			 /*in */ MESSAGE_SIZE_TYPE          MAX_MESSAGE_SIZE,
			 /*in */ PORT_DIRECTION_TYPE        PORT_DIRECTION,
			 /*in */ SYSTEM_TIME_TYPE           REFRESH_PERIOD,
			 /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
			 /*out*/ RETURN_CODE_TYPE           *RETURN_CODE )
{
	 pok_port_direction_t core_direction;
	 pok_port_id_t        core_id;
	 pok_ret_t            core_ret;
	 pok_thread_attr_t     attr;
	 uint32_t							process_id;

	 pok_thread_id(&process_id);
	 core_ret = pok_thread_status (process_id, &attr);
	 if (attr.state == NORMAL)
		 {
			 *RETURN_CODE = INVALID_MODE;
			 return ;
		 }
	 if (MAX_MESSAGE_SIZE <= 0 || REFRESH_PERIOD < 0)
		 {
			 *RETURN_CODE = INVALID_CONFIG;
			 return ;
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
				 *RETURN_CODE = INVALID_PARAM;
				 return;
	 }

	 core_ret = pok_port_sampling_create (SAMPLING_PORT_NAME, MAX_MESSAGE_SIZE, core_direction, REFRESH_PERIOD, &core_id);

	 *SAMPLING_PORT_ID = core_id;

	 *RETURN_CODE = core_ret;
}

void WRITE_SAMPLING_MESSAGE (
			 /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
			 /*in */ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,     /* by reference */
			 /*in */ MESSAGE_SIZE_TYPE          LENGTH,
			 /*out*/ RETURN_CODE_TYPE           *RETURN_CODE )
{
	 pok_ret_t core_ret;

	 if (LENGTH <= 0)
		 {
			 *RETURN_CODE = INVALID_PARAM;
			 return;
		 }
	 core_ret = pok_port_sampling_write (SAMPLING_PORT_ID, MESSAGE_ADDR, LENGTH);
	 *RETURN_CODE = core_ret;
}

void READ_SAMPLING_MESSAGE (
			 /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
			 /*out*/ MESSAGE_ADDR_TYPE          MESSAGE_ADDR,
			 /*out*/ MESSAGE_SIZE_TYPE          *LENGTH,
			 /*out*/ VALIDITY_TYPE              *VALIDITY,
			 /*out*/ RETURN_CODE_TYPE           *RETURN_CODE )
{
	 pok_ret_t core_ret;
	 core_ret = pok_port_sampling_read (SAMPLING_PORT_ID, MESSAGE_ADDR, (pok_port_size_t*) LENGTH, (pok_bool_t*) VALIDITY);
	 *RETURN_CODE = core_ret;
}

void GET_SAMPLING_PORT_ID (
			 /*in */ SAMPLING_PORT_NAME_TYPE    SAMPLING_PORT_NAME,
			 /*out*/ SAMPLING_PORT_ID_TYPE      *SAMPLING_PORT_ID,
			 /*out*/ RETURN_CODE_TYPE           *RETURN_CODE )
{
	pok_ret_t core_ret;
	pok_port_id_t id;

	core_ret = pok_port_sampling_id(SAMPLING_PORT_NAME, &id);
	*SAMPLING_PORT_ID = id;
	*RETURN_CODE = core_ret;
}

void GET_SAMPLING_PORT_STATUS (
			 /*in */ SAMPLING_PORT_ID_TYPE      SAMPLING_PORT_ID,
			 /*out*/ SAMPLING_PORT_STATUS_TYPE  *SAMPLING_PORT_STATUS,
			 /*out*/ RETURN_CODE_TYPE           *RETURN_CODE )
{
	pok_ret_t core_ret;
	pok_port_sampling_status_t status;

	core_ret = pok_port_sampling_status(SAMPLING_PORT_ID, &status);
	SAMPLING_PORT_STATUS->REFRESH_PERIOD = status.refresh;
	SAMPLING_PORT_STATUS->MAX_MESSAGE_SIZE = status.size;
	SAMPLING_PORT_STATUS->PORT_DIRECTION = status.direction;
	SAMPLING_PORT_STATUS->LAST_MSG_VALIDITY = status.validity;
	*RETURN_CODE = core_ret;
}

#endif
