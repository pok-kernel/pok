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

#include <core/dependencies.h>

#include <core/thread.h>
#include <arinc653/arincutils.h>
#include <arinc653/types.h>
#include <arinc653/process.h>
#include <libc/string.h>

uint32_t pok_arinc653_processes[POK_CONFIG_NB_THREADS];
uint32_t pok_arinc653_processes_nb;


void GET_PROCESS_ID (PROCESS_NAME_TYPE process_name[MAX_NAME_LENGTH],
										 PROCESS_ID_TYPE   *process_id,
										 RETURN_CODE_TYPE  *return_code )
{
	int id;

	if ((id = process_name_exist(process_name)) == 0)
		{
			*process_id = id;
			*return_code = INVALID_CONFIG;
		}
	else
		{
			*process_id = id;
			*return_code = NO_ERROR;
		}
}

void GET_MY_ID (PROCESS_ID_TYPE   *process_id,
		RETURN_CODE_TYPE  *return_code )
{
	pok_ret_t         core_ret;
	uint32_t			thread_id;

	core_ret = pok_thread_id (&thread_id);
	if (core_ret != 0)
		*return_code = INVALID_MODE;
	*process_id = thread_id;
	*return_code = NO_ERROR;
}

void GET_PROCESS_STATUS (PROCESS_ID_TYPE     process_id,
												 PROCESS_STATUS_TYPE *process_status,
												 RETURN_CODE_TYPE    *return_code )
{
	pok_thread_attr_t	attr;
	pok_ret_t		core_ret;

	uint32_t          core_process_id = pok_arinc653_processes[process_id];
	
	core_ret = pok_thread_status (core_process_id, &attr);
	if (core_ret ==  POK_ERRNO_PARAM)
		{
			*return_code =  INVALID_PARAM;
			return ;
		}
	process_status->DEADLINE_TIME = attr.deadline;
	process_status->PROCESS_STATE = attr.state;
	strcpy(process_status->ATTRIBUTES.NAME, arinc_process_attribute[core_process_id].NAME);
	process_status->ATTRIBUTES.BASE_PRIORITY = arinc_process_attribute[core_process_id].BASE_PRIORITY;
	process_status->ATTRIBUTES.DEADLINE = HARD;
	process_status->CURRENT_PRIORITY = attr.priority;
	process_status->ATTRIBUTES.PERIOD = attr.period;
	process_status->ATTRIBUTES.TIME_CAPACITY = attr.time_capacity;
	process_status->ATTRIBUTES.ENTRY_POINT = attr.entry;
	process_status->ATTRIBUTES.STACK_SIZE = attr.stack_size;
	*return_code = NO_ERROR;
}


void CREATE_PROCESS (PROCESS_ATTRIBUTE_TYPE  *attributes,
										 PROCESS_ID_TYPE         *process_id,
										 RETURN_CODE_TYPE        *return_code )
{
	 pok_thread_attr_t core_attr;
	 pok_ret_t         core_ret;
	 uint32_t          core_process_id;

	 if (process_name_exist(&attributes->NAME))
		 {
			 *return_code = NO_ACTION;
			 return;
		 }
	 if (attributes->BASE_PRIORITY > MAX_PRIORITY_VALUE || attributes->BASE_PRIORITY < MIN_PRIORITY_VALUE)
		 {
			 *return_code = INVALID_PARAM;
			 return;
		 }
	 core_attr.priority        = (uint8_t) attributes->BASE_PRIORITY;
	 core_attr.entry           = attributes->ENTRY_POINT;
	 core_attr.period          = attributes->PERIOD;
	 core_attr.deadline        = attributes->DEADLINE;
	 core_attr.time_capacity   = attributes->TIME_CAPACITY;
	 core_attr.stack_size      = attributes->STACK_SIZE;

	 core_ret = pok_thread_create (&core_process_id, &core_attr);
	 *return_code = core_ret;
	 if(core_ret==POK_ERRNO_OK) {
	   pok_arinc653_processes_nb++;
	   pok_arinc653_processes[pok_arinc653_processes_nb] = core_process_id;
	   arinc_process_attribute[pok_arinc653_processes_nb].BASE_PRIORITY = attributes->BASE_PRIORITY;
	   strcpy(arinc_process_attribute[pok_arinc653_processes_nb].NAME, attributes->NAME);
	   *process_id = pok_arinc653_processes_nb;
	 }
	 else
	   return;

   // ARINC specifies that threads shall be created in the DORMANT state
   core_ret = pok_thread_suspend_target(core_process_id);
   *return_code = core_ret;
}

void STOP_SELF ()
{
	 pok_thread_stop_self ();
}


void SET_PRIORITY (PROCESS_ID_TYPE  process_id,
									 PRIORITY_TYPE    priority,
									 RETURN_CODE_TYPE *return_code )
{
	pok_thread_attr_t core_attr;
	pok_ret_t         core_ret;

	uint32_t          core_process_id = pok_arinc653_processes[process_id];
	
	core_ret = pok_thread_status (core_process_id, &core_attr);
	if (core_ret != POK_ERRNO_OK)
		{
			*return_code =  INVALID_PARAM;
			return;
		}
	if (priority > MAX_PRIORITY_VALUE || priority < MIN_PRIORITY_VALUE)
		{
			*return_code = INVALID_PARAM;
			return;
		}
	if (core_attr.state == DORMANT)
		{
			*return_code = INVALID_MODE;
			return;
		}
	core_ret = pok_thread_set_priority(core_process_id, priority);
	*return_code = core_ret;
}

#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE
void SUSPEND_SELF (SYSTEM_TIME_TYPE time_out,
									 RETURN_CODE_TYPE *return_code )
{
	 (void) time_out;
	 *return_code = NOT_AVAILABLE;
}

#endif

void SUSPEND (PROCESS_ID_TYPE    process_id,
							RETURN_CODE_TYPE   *return_code )
{
	pok_thread_attr_t  attr;
	pok_ret_t    core_ret;

	uint32_t          core_process_id = pok_arinc653_processes[process_id];
	
	core_ret = pok_thread_status (core_process_id, &attr);
	if (attr.state == DORMANT)
		{
			*return_code = INVALID_MODE;
			return ;
		}
	if (attr.period == INFINITE_TIME_VALUE)
		{
			*return_code = INVALID_MODE;
			return ;
		}
	if (attr.state == WAITING)
		{
			*return_code = NO_ACTION;
			return ;
		}
	core_ret = pok_thread_suspend_target (core_process_id);
	*return_code = core_ret;
}

void RESUME (PROCESS_ID_TYPE     process_id,
						 RETURN_CODE_TYPE    *return_code )
{
	pok_thread_attr_t  attr;
	pok_ret_t    core_ret;

	uint32_t          core_process_id = pok_arinc653_processes[process_id];
	
	core_ret = pok_thread_status (core_process_id, &attr);
	if (core_ret != 0)
		{
			*return_code = INVALID_PARAM;
			return ;
		}
	if (attr.state == DORMANT)
		{
			*return_code = INVALID_MODE;
			return ;
		}
	if (attr.period == INFINITE_TIME_VALUE)
		{
			*return_code = INVALID_MODE;
			return ;
		}
	if (attr.state != WAITING)
		{
			*return_code = INVALID_MODE;
			return ;
		}
	core_ret = pok_thread_resume (core_process_id);
	*return_code = core_ret;
}

void START (PROCESS_ID_TYPE   process_id,
					 RETURN_CODE_TYPE   *return_code )
{
  DELAYED_START(process_id,0,return_code);
}

void STOP (PROCESS_ID_TYPE    process_id,
	   RETURN_CODE_TYPE *return_code )
{
  uint32_t          core_process_id = pok_arinc653_processes[process_id];
  *return_code = pok_thread_stop(core_process_id);
}

void DELAYED_START (PROCESS_ID_TYPE   process_id,
				SYSTEM_TIME_TYPE  delay_time,
				RETURN_CODE_TYPE *return_code )
{
  pok_thread_attr_t     attr;
  pok_ret_t		core_ret;

  uint32_t          core_process_id = pok_arinc653_processes[process_id];
  
  core_ret = pok_thread_status (core_process_id, &attr);
  if (core_ret != POK_ERRNO_OK)
    {
      *return_code = INVALID_PARAM;
      return;
    }
    if (attr.state != DORMANT)
   {
     *return_code = NO_ACTION;
    return;
   }
  if (delay_time == INFINITE_TIME_VALUE)
    {
      *return_code = INVALID_PARAM;
      return;
    }
  /*if ((int)attr.period != INFINITE_TIME_VALUE && delay_time >= attr.period)
  {
    *return_code = INVALID_PARAM;
    return;
  }*/
  core_ret = pok_thread_delayed_start(core_process_id, delay_time);
  if (core_ret == POK_ERRNO_OK) {
    *return_code = NO_ERROR;
  }else {
    *return_code = INVALID_PARAM;
  }
}

#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE
void LOCK_PREEMPTION (LOCK_LEVEL_TYPE     *lock_level,
											RETURN_CODE_TYPE    *return_code )
{
	 (void) lock_level;
	 *return_code = NOT_AVAILABLE;
}

void UNLOCK_PREEMPTION (LOCK_LEVEL_TYPE   *lock_level,
												RETURN_CODE_TYPE  *return_code )

{
	 (void) lock_level;
	 *return_code = NOT_AVAILABLE;
}
#endif

#endif
