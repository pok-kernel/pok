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

/* @(#)w_j0.c 5.1 93/09/24 */
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
 * wrapper j0(double x), y0(double x)
 */

#include "math_private.h"
#include <libm.h>

double j0(double x) /* wrapper j0 */
{
#ifdef _IEEE_LIBM
  return __ieee754_j0(x);
#else
  double z = __ieee754_j0(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x))
    return z;
  if (fabs(x) > X_TLOSS) {
    return __kernel_standard(x, x, 34); /* j0(|x|>X_TLOSS) */
  } else
    return z;
#endif
}

double y0(double x) /* wrapper y0 */
{
#ifdef _IEEE_LIBM
  return __ieee754_y0(x);
#else
  double z;
  z = __ieee754_y0(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x))
    return z;
  if (x <= 0.0) {
    if (x == 0.0)
      /* d= -one/(x-x); */
      return __kernel_standard(x, x, 8);
    else
      /* d = zero/(x-x); */
      return __kernel_standard(x, x, 9);
  }
  if (x > X_TLOSS) {
    return __kernel_standard(x, x, 35); /* y0(x>X_TLOSS) */
  } else
    return z;
#endif
}

#endif
