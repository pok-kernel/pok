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

/* e_scalbf.c -- float version of e_scalb.c.
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
#include <libm.h>

#ifdef _SCALB_INT
float __ieee754_scalbf(float x, int fn)
#else
float __ieee754_scalbf(float x, float fn)
#endif
{
#ifdef _SCALB_INT
  return scalbnf(x, fn);
#else
  if (isnanf(x) || isnanf(fn))
    return x * fn;
  if (!finitef(fn)) {
    if (fn > (float)0.0)
      return x * fn;
    else
      return x / (-fn);
  }
  if (rintf(fn) != fn)
    return (fn - fn) / (fn - fn);
  if (fn > (float)65000.0)
    return scalbnf(x, 65000);
  if (-fn > (float)65000.0)
    return scalbnf(x, -65000);
  return scalbnf(x, (int)fn);
#endif
}
#endif
