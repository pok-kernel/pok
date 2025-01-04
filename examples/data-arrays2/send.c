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
 *                                      Copyright (c) 2007-2025 POK team
 */

#include <gtypes.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include <types.h>

#define MYSTRING "Bonjour"

void user_send(arraytest__buffer *t) {
  char *toto = (char *)*t;
  memcpy(toto, MYSTRING, 7);
  toto[7] = '\0';

  printf("Sent value %s\n", (char *)*t);
}
