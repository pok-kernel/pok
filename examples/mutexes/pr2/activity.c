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
 *                                      Copyright (c) 2007-2022 POK team
 */

#include <core/thread.h>
#include <libc/stdio.h>
void *pinger_job() {
  while (1) {
    printf("Hello, i'm task one in partition two\n");
    pok_thread_sleep(2000000);
  }
}
