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

#include <core/shutdown.h>
#include <libc/stdio.h>

int main() {
  char *s = "Test printf";
  char c = ':';
  unsigned long long int i = 0x87654321deadbeef;
  printf("64 bits number:0x%llx\n", i);
  printf("%s%c %%llx: %llx - %%lx: %lx - %%x: %x - %%hx: %hx - %%hhx: %hhx\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%llo: %llo - %%lo: %lo - %%o: %o - %%ho: %ho - %%hho: %hho\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%llu: %llu - %%lu: %lu - %%u: %u - %%hu: %hu - %%hhu: %hhu\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%lld: %lld - %%ld: %ld - %%d: %d - %%hd: %hd - %%hhd: %hhd\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%lli: %lli - %%li: %li - %%i: %i - %%hi: %hi - %%hhi: %hhi\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);

  i = 0xdeadbeef87654321;
  printf("64 bits number:0x%llx\n", i);
  printf("%s%c %%llx: %llx - %%lx: %lx - %%x: %x - %%hx: %hx - %%hhx: %hhx\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%llo: %llo - %%lo: %lo - %%o: %o - %%ho: %ho - %%hho: %hho\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%llu: %llu - %%lu: %lu - %%u: %u - %%hu: %hu - %%hhu: %hhu\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%lld: %lld - %%ld: %ld - %%d: %d - %%hd: %hd - %%hhd: %hhd\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);
  printf("%s%c %%lli: %lli - %%li: %li - %%i: %i - %%hi: %hi - %%hhi: %hhi\n",
         s, c, i, (long int)i, (int)i, (short)i, (char)i);

  printf("This is PI: %f\n", 3.141592653589793);

  pok_shutdown();
}
