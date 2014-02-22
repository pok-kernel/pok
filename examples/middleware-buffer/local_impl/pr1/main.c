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
#include <core/thread.h>
#include "activity.h"


void main ()
{
  uint8_t tid;
  pok_thread_attr_t     tattr;

  tattr.priority = 42;
  tattr.entry = pinger_job;

  pok_thread_create (&tid, &tattr);

  tattr.priority = 42;
  tattr.entry = pingme_job;

  pok_thread_create (&tid, &tattr);

  pok_thread_sleep (1000000);
}


