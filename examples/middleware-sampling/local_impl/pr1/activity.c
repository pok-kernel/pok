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


#include <libc/stdio.h>
#include <errno.h>
#include <middleware/port.h>
#include "types.h"
uint8_t port_pinger_data_source_id;
void* pinger_job ()
{
   int ret;
   simple_type data_source_request_var;
  ret = pok_port_sampling_create ("dataout", 4, POK_PORT_DIRECTION_OUT, 10, &port_pinger_data_source_id);
  printf("Create port, id=%d,return=%d\n",port_pinger_data_source_id,ret);
   while (1)
   {
      do_ping_spg(&(data_source_request_var));
      ret = pok_port_sampling_write (port_pinger_data_source_id, &data_source_request_var, sizeof (data_source_request_var));
      if (ret != POK_ERRNO_OK)
      {
         printf("SEND ERROR\n");
      }
      pok_thread_sleep (2000);
   }
}

