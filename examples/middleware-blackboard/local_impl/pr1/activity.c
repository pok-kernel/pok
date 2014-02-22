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


#include <middleware/blackboard.h>
#include "types.h"
#include <types.h>

pok_blackboard_id_t blackboard_id;

void* pinger_job ()
{
   int ret;
   simple_type data_source_request_var;

   printf ("Debut premier thread\n");

  ret = pok_blackboard_create ("myblackboard", 4, &blackboard_id);
  printf("Create blackboard for sending task, return=%d\n",ret);

   while (1)
   {
      do_ping_spg(&(data_source_request_var));
      ret = pok_blackboard_display (blackboard_id, &data_source_request_var, sizeof (data_source_request_var));
      printf("Display blackboard value=%d, return=%d\n", data_source_request_var, ret);
      pok_thread_sleep (1000);
   }
}

void* pingme_job ()
{
   simple_type data_sink_request_var;
   int ret;
   uint8_t length;
   uint8_t valid;

   printf ("Debut second thread\n");

   while (1)
   {
      ret = pok_blackboard_read (blackboard_id, 0, &data_sink_request_var, &length);
      if (ret == 0)
      {
         ping_spg(data_sink_request_var);
      }
      else
      {
         printf("No data to read on blackboard, return = %d\n", ret);
      }
      pok_thread_sleep (500);
   }
}


