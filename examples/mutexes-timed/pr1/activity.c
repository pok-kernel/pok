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

void* pinger_job ()
{
   pok_ret_t ret;
   while (1)
   {
      printf ("P1T1: before lock\n");
      ret = pok_mutex_lock (mid);
      printf ("P1T1: mutex lock, ret=%d\n", ret);
      printf ("P1T1: before sleep\n");
      pok_thread_sleep (10000);
      printf ("P1T1: after sleep\n");
      ret = pok_mutex_unlock (mid);
      printf ("P1T1: mutex unlock, ret=%d\n", ret);
   }
}

void* pinger_job2 ()
{
   pok_ret_t ret;
   while (1)
   {
      printf ("P1T2: before trylock\n");
      ret = pok_mutex_trylock (mid, 1000);
      printf ("P1T2: mutex trylock, ret=%d\n", ret);
      if (ret == POK_ERRNO_TIMEOUT)
      {
         printf ("P1T2: TIMEOUT\n");
      }
      printf ("P1T2: before sleep\n");
      pok_thread_sleep (2000);
      printf ("P1T2: after sleep\n");
      if (ret != POK_ERRNO_TIMEOUT)
      {
         ret = pok_mutex_unlock (mid);
         printf ("P1T2: mutex unlock, ret=%d\n", ret);
      }
   }
}
