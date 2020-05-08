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

#include <gtypes.h>
#include <libc/stdio.h>
#include <libm.h>
#include <types.h>

#define MYSTRING "Bonjour"

int bla = 67;

void user_send(arraytest__buffer *t) {
  char *toto = (char *)*t;
  memcpy(toto, MYSTRING, 7);
  toto[7] = bla;
  toto[8] = '\0';

  bla++;

  printf("Sent value %s\n", (char *)*t);
}
