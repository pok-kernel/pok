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
 * wrapper sqrt(x)
 */

#ifdef POK_NEEDS_LIBMATH

#include "math_private.h"
#include <libm.h>

double sqrt(double x) /* wrapper sqrt */
{
#ifdef _IEEE_LIBM
  return __ieee754_sqrt(x);
#else
  double z;
  z = __ieee754_sqrt(x);
  if (isnan(x))
    return z;
  if (x < 0.0) {
    return -1.0;
  } else
    return z;
#endif
}

#endif
