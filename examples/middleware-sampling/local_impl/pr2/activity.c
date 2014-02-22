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
#include <middleware/port.h>
#include <core/time.h>
#include <types.h>
#include "types.h"

uint8_t port_pingme_data_sink;

void* pingme_job ()
{
   simple_type data_sink_request_var = 0;
   int ret;
   uint8_t length;
  pok_time_t mytime;
   bool_t valid;
  ret = pok_port_sampling_create ("datain", 4, POK_PORT_DIRECTION_IN, 10, &port_pingme_data_sink);
  printf("Create port, id = %d , return = %d\n",port_pingme_data_sink, ret);

   while (1)
   {
      pok_time_compute_deadline (pok_time_milliseconds (500), &mytime);
      ret = pok_port_sampling_read (port_pingme_data_sink, &data_sink_request_var, &length, &valid);
      if (ret == 0)
      {
         ping_spg(data_sink_request_var);
      }
      else
      {
         printf("No data to print, return = %d\n", ret);
      }
  //    pok_thread_sleep (2000);
     pok_thread_sleep_until (mytime); 
   }
}

