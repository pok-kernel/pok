/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2020 POK team
 */

#include <core/thread.h>
#include <libc/stdio.h>
void *pinger_job() {
  pok_ret_t ret;
  while (1) {
    printf("P2T1: before lock\n");
    ret = pok_mutex_lock(0);
    printf("P2T1: Try to lock mutex 0, result=%d\n", ret);
    pok_thread_sleep(10000000);
  }
}
