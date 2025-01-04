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

/* s_sinf.c -- float version of s_sin.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifdef POK_NEEDS_LIBMATH

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#if 0 /* notyet */
#ifdef __weak_alias
__weak_alias(sinf, _sinf)
#endif
#endif

float sinf(float x) {
  float y[2], z = 0.0;
  int32_t n, ix;

  GET_FLOAT_WORD(ix, x);

  /* |x| ~< pi/4 */
  ix &= 0x7fffffff;
  if (ix <= 0x3f490fd8)
    return __kernel_sinf(x, z, 0);

  /* sin(Inf or NaN) is NaN */
  else if (ix >= 0x7f800000)
    return x - x;

  /* argument reduction needed */
  else {
    n = __ieee754_rem_pio2f(x, y);
    switch (n & 3) {
    case 0:
      return __kernel_sinf(y[0], y[1], 1);
    case 1:
      return __kernel_cosf(y[0], y[1]);
    case 2:
      return -__kernel_sinf(y[0], y[1], 1);
    default:
      return -__kernel_cosf(y[0], y[1]);
    }
  }
}

#endif
