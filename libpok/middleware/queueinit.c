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


#ifdef POK_NEEDS_MIDDLEWARE
#if defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)

#include <middleware/queue.h>

extern pok_queue_t pok_queues[POK_CONFIG_NB_PARTITIONS];
extern char pok_queues_data[POK_CONFIG_NB_PARTITIONS][1024];

void pok_queue_init ()
{
   uint8_t i;

   for (i = 0 ; i < POK_CONFIG_NB_PARTITIONS ; i++)
   {
      pok_queues[i].size = 1024;
      pok_queues[i].available_size = 1024;
      pok_queues[i].data = pok_queues_data[i];
   }

}
#endif
#endif
