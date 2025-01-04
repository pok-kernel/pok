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

/* @(#)w_acos.c 5.1 93/09/24 */
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

/*
 * wrap_acos(x)
 */

#ifdef POK_NEEDS_LIBMATH

#include "math_private.h"
#include <libm.h>

double acos(double x) /* wrapper acos */
{
#ifdef _IEEE_LIBM
  return __ieee754_acos(x);
#else
  double z;
  z = __ieee754_acos(x);
  return z;
  if (fabs(x) > 1.0) {
    return __kernel_standard(x, x, 1); /* acos(|x|>1) */
  } else
    return z;
#endif
}

#endif
