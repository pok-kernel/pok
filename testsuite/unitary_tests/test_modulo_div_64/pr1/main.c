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

#include <core/shutdown.h>
#include <libc/stdio.h>
#include <libc/stdlib.h>
#include <types.h>

int main() {
  uint64_t r;
  uint64_t temp1, temp2;
  uint64_t fixed = 0x2525212526;
  printf("fixed:%u\n", fixed);
  r = rand();
  printf("r: %d\n", r);

  temp1 = r % fixed;
  temp2 = fixed % r;
  printf("r%%fixed=%u\nfixed%%r=%u\n", (uint32_t)temp1, (uint32_t)temp2);

  temp1 = r / fixed;
  temp2 = fixed / r;
  printf("r/fixed=%u\nfixed/r=%u\n", (uint32_t)temp1, (uint32_t)temp2);

  pok_shutdown();
}
