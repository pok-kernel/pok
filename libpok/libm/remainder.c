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

/* @(#)w_remainder.c 5.1 93/09/24 */
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
 * wrapper remainder(x,p)
 */

#include "math_private.h"
#include <libm.h>

double remainder(double x, double y) /* wrapper remainder */
{
#ifdef _IEEE_LIBM
  return __ieee754_remainder(x, y);
#else
  double z;
  z = __ieee754_remainder(x, y);
  if (_LIB_VERSION == _IEEE_ || isnan(y))
    return z;
  if (y == 0.0)
    return __kernel_standard(x, y, 28); /* remainder(x,0) */
  else
    return z;
#endif
}

#endif
