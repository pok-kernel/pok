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

/*
 * Written by J.T. Conklin <jtc@NetBSD.org>.
 * Public domain.
 */

#ifdef POK_NEEDS_LIBMATH

/*
 * isinf(x) returns 1 is x is inf, else 0;
 * no branching!
 */

#include "math_private.h"
#include <libm.h>

int isinf(double x) {
  int32_t hx, lx;
  EXTRACT_WORDS(hx, lx, x);
  hx &= 0x7fffffff;
  hx ^= 0x7ff00000;
  hx |= lx;
  return (hx == 0);
}

#endif
