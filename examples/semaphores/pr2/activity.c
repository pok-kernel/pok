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
  while (1) {
    printf("P2T1: begin of task\n");
    pok_thread_sleep(5000000);
  }
}
