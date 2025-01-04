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

/* w_gammaf_r.c -- float version of w_gamma_r.c.
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
 * wrapper float gammaf_r(float x, int *signgamp)
 */

#include "math_private.h"
#include <libm.h>

float gammaf_r(float x, int *signgamp) /* wrapper lgammaf_r */
{
#ifdef _IEEE_LIBM
  return __ieee754_lgammaf_r(x, signgamp);
#else
  float y;
  y = __ieee754_lgammaf_r(x, signgamp);
  if (_LIB_VERSION == _IEEE_)
    return y;
  if (!finitef(y) && finitef(x)) {
    if (floorf(x) == x && x <= (float)0.0)
      /* gammaf pole */
      return (float)__kernel_standard((double)x, (double)x, 141);
    else
      /* gamma overflow */
      return (float)__kernel_standard((double)x, (double)x, 140);
  } else
    return y;
#endif
}

#endif
