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

/* w_fmodf.c -- float version of w_fmod.c.
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
 * wrapper fmodf(x,y)
 */

#include "math_private.h"
#include <libm.h>

float fmodf(float x, float y) /* wrapper fmodf */
{
#ifdef _IEEE_LIBM
  return __ieee754_fmodf(x, y);
#else
  float z;
  z = __ieee754_fmodf(x, y);
  if (_LIB_VERSION == _IEEE_ || isnanf(y) || isnanf(x))
    return z;
  if (y == (float)0.0) {
    /* fmodf(x,0) */
    return (float)__kernel_standard((double)x, (double)y, 127);
  } else
    return z;
#endif
}

#endif
