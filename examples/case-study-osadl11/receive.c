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
 *                                      Copyright (c) 2007-2021 POK team
 */

#include <libc/stdio.h>
#include <types.h>
static int step = 0;

void user_receive(int t) {
  int d;
  d = (t + 1) % 3;
  printf("Step %d\n", step++);
  printf("Received value %d\n", t);
  printf("Computed value %d\n", t / d);
}
