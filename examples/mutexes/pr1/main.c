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
#include <core/thread.h>
#include <core/mutex.h>
#include <types.h>
#include "activity.h"

uint8_t mid;

int main ()
{
  uint8_t tid;
  pok_ret_t ret;
  pok_thread_attr_t     tattr;

  tattr.priority = 42;
  tattr.entry = pinger_job;

  ret = pok_thread_create(&tid , &tattr);

  tattr.priority = 42;
  tattr.entry = pinger_job2;

  ret = pok_thread_create(&tid , &tattr);

  ret = pok_mutex_create(&mid , NULL);
  printf("pok_mutex_create return=%d, mid=%d\n", ret, mid);

  pok_thread_wait_infinite ();

   return (0);
}
