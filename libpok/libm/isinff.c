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

/*
 * Written by J.T. Conklin <jtc@NetBSD.org>.
 * Public domain.
 */

/*
 * isinff(x) returns 1 is x is inf, else 0;
 * no branching!
 */

#ifdef POK_NEEDS_LIBMATH

#include "math_private.h"
#include <libm.h>

int isinff(float x) {
  int32_t ix;
  GET_FLOAT_WORD(ix, x);
  ix &= 0x7fffffff;
  ix ^= 0x7f800000;
  return (ix == 0);
}

#endif
