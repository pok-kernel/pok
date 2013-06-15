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
 * Created by julien on Wed Aug 12 15:48:00 2009 
 */

#include <gtypes.h>
#include <libc/stdio.h>


void user_queued_buffer_producer (sigada09__integer_i* produced_data)
{
   static int produced = 0;
   (void) produced_data;
   printf ("I produce data %d\n", produced);
   *produced_data = produced++;
}

void user_queued_buffer_consumer (sigada09__integer_i consumed_data)
{
   printf ("I consume data %d\n", consumed_data);
}

