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


#include <core/thread.h>
#include <core/partition.h>
#include <types.h>
#include "activity.h"


int main ()
{
  uint8_t tid;
  int ret;
  pok_thread_attr_t     tattr;

  tattr.priority = 42;
  tattr.entry = pinger_job;

  ret = pok_thread_create(&tid , &tattr);
  printf ("[P2] thread create returns=%d\n", ret);

  pok_partition_set_mode (POK_PARTITION_MODE_NORMAL);
  pok_thread_wait_infinite ();

  return (1);
}


