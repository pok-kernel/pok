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

#include <libc/stdio.h>
#include <types.h>

int toto = 0;
uint32_t cs;
uint32_t ds;

void user_send(int *t) {
  *t = toto;

  asm("movl %%cs, %0" : "=r"(cs));

  asm("movl %%ds, %0" : "=r"(ds));

  printf("[PART1] Send value %d CS=%d, DS=%d\n", toto, cs, ds);

  toto++;
}
