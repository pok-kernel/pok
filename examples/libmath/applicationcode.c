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
#include <libc/stdlib.h>
#include <libm.h>

void compute() {
  int r;
  double s = 0.0;
  double test = 4.000001;

  s = sqrt(16.0);
  r = rand();
  printf("r       = %d\n", r);
  printf("r*r     =  %d\n", (int)pow((double)r, 2.0));
  printf("sqrt 16 = %d\n", (int)s);

  printf("sqrt 13 = %f\n", sqrt(13.0));

  printf("4.000001 = %f\n", test);
  test = 1.234567;
  printf("1.234567 = %f\n", test);
}
