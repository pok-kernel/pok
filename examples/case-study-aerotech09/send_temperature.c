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
 *                                      Copyright (c) 2007-2024 POK team
 */

#include <libc/stdio.h>
#include <types.h>

int toto = 0;

void user_send_temperature(int *t) {
  *t = toto;
  printf("Send temperature %d\n", toto);
  toto++;
}
