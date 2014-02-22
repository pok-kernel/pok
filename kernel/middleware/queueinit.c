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


#if defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)

#include <middleware/queue.h>

extern pok_queue_t pok_queue;
extern char        pok_queue_data[1024];

void pok_queue_init ()
{
   pok_queue.size = 1024;
   pok_queue.available_size = 1024;
   pok_queue.data = pok_queue_data;
}
#endif
