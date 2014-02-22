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

extern uint8_t mid;
uint8_t val;

void* pinger_job ()
{
   pok_ret_t ret;
   while (1)
   {
      ret = pok_mutex_lock (mid);
      printf("mutex lock, ret=%d\n", ret);
      val = 10;
      printf ("Hello, I'm task one in partition one, before sleep val=%d\n", val);
      pok_thread_sleep (100);
      printf ("Hello, I'm task one in partition one, after sleep val=%d\n", val);
      ret = pok_mutex_unlock (mid);
      printf("mutex unlock, ret=%d\n", ret);
      pok_thread_sleep (1000);
   }
}

void* pinger_job2 ()
{
   pok_ret_t ret;
   while (1)
   {
      ret = pok_mutex_lock (mid);
      printf("mutex lock, ret=%d\n", ret);
      printf ("Hello, I'm task two in partition one, value before change=%d\n", val);
      val = 5;
      pok_thread_sleep (200);
      printf ("Hello, I'm task two in partition one, value after change=%d\n", val);
      ret = pok_mutex_unlock (mid);
      printf("mutex unlock, ret=%d\n", ret);
      pok_thread_sleep (500);
   }
}
