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


#include <types.h>
#include <errno.h>
#include <core/thread.h>
#include <middleware/port.h>
#include "types.h"

uint8_t port_pingme_data_sink;

void* pingme_job ()
{
   simple_type data_sink_request_var;
   int ret;
   uint8_t length;
   uint64_t toto;
   toto = 0;

  ret = pok_port_queueing_create ("datain", 8, POK_PORT_DIRECTION_IN, POK_PORT_QUEUEING_DISCIPLINE_FIFO, &port_pingme_data_sink);
  printf("P2T1: Create port, return = %d\n",ret);

   while (1)
   {
      ret = pok_port_queueing_receive (port_pingme_data_sink, 250, 4, &data_sink_request_var, &length);

      if (ret == POK_ERRNO_OK)
      {
         ping_spg(data_sink_request_var);
      }
      else
      {
         printf("P2T1: RECEIVE ERROR, ret=%d\n", ret);
      }
      pok_thread_sleep (500);
   }
}

