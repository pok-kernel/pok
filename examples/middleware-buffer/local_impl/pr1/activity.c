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


#include <middleware/buffer.h>
#include "types.h"
#include <types.h>
#include <core/thread.h>
#include <libc/stdio.h>

void do_ping_spg (simple_type*);
void ping_spg (simple_type);
pok_buffer_id_t buffer_id;

void* pinger_job ()
{
   int ret = 0;
   simple_type data_source_request_var;
  ret = pok_buffer_create ("mybuffer", 8, 4, POK_BUFFER_DISCIPLINE_FIFO, &buffer_id);

   while (1)
   {
      do_ping_spg(&(data_source_request_var));
      ret = pok_buffer_send (buffer_id, &data_source_request_var, sizeof (data_source_request_var), 5);
      printf("buffer_send ret=%d\n", ret);
      pok_thread_sleep (1000);
   }
}

void* pingme_job ()
{
   simple_type data_sink_request_var;
   int ret;
   uint8_t length;

   while (1)
   {
      ret = pok_buffer_receive (buffer_id, 250, &data_sink_request_var, &length);
      if (ret == 0)
      {
         ping_spg(data_sink_request_var);
      }
      else
      {
         printf("No data to read on buffer, return = %d\n", ret);
      }
      pok_thread_sleep (3000);
   }
}


