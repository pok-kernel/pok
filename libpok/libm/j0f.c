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

/* w_j0f.c -- float version of w_j0.c.
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

/*
 * wrapper j0f(float x), y0f(float x)
 */

#include "math_private.h"
#include <libm.h>

float j0f(float x) /* wrapper j0f */
{
#ifdef _IEEE_LIBM
  return __ieee754_j0f(x);
#else
  float z = __ieee754_j0f(x);
  if (_LIB_VERSION == _IEEE_ || isnanf(x))
    return z;
  if (fabsf(x) > (float)X_TLOSS) {
    /* j0f(|x|>X_TLOSS) */
    return (float)__kernel_standard((double)x, (double)x, 134);
  } else
    return z;
#endif
}

float y0f(float x) /* wrapper y0f */
{
#ifdef _IEEE_LIBM
  return __ieee754_y0f(x);
#else
  float z;
  z = __ieee754_y0f(x);
  if (_LIB_VERSION == _IEEE_ || isnanf(x))
    return z;
  if (x <= (float)0.0) {
    if (x == (float)0.0)
      /* d= -one/(x-x); */
      return (float)__kernel_standard((double)x, (double)x, 108);
    else
      /* d = zero/(x-x); */
      return (float)__kernel_standard((double)x, (double)x, 109);
  }
  if (x > (float)X_TLOSS) {
    /* y0(x>X_TLOSS) */
    return (float)__kernel_standard((double)x, (double)x, 135);
  } else
    return z;
#endif
}

#endif
