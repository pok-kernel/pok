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


#include <middleware/port.h>
#include "types.h"
#include <types.h>
#include <errno.h>
uint8_t port_pinger_data_source_id;
void* pinger_job ()
{
   int ret;
   simple_type data_source_request_var;

  ret = pok_port_queueing_create ("dataout", 8, POK_PORT_DIRECTION_OUT, POK_PORT_QUEUEING_DISCIPLINE_FIFO, &port_pinger_data_source_id);
  printf("P1T1: create queue, return=%d\n",ret);

  while (1)
   {
      do_ping_spg(&(data_source_request_var));
      ret = pok_port_queueing_send (port_pinger_data_source_id, &data_source_request_var, 4 , 200);
      if (ret != POK_ERRNO_OK)
      {
         printf ("P1T1: SEND ERROR, ret=%d\n", ret);
      }
      pok_thread_sleep (1000);
   }
}

