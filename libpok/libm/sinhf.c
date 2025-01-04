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

/* w_sinhf.c -- float version of w_sinh.c.
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
 * wrapper sinhf(x)
 */

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#ifdef __weak_alias
__weak_alias(sinhf, _sinhf)
#endif

    float sinhf(float x) /* wrapper sinhf */
{
#ifdef _IEEE_LIBM
  return __ieee754_sinhf(x);
#else
  float z;
  z = __ieee754_sinhf(x);
  if (_LIB_VERSION == _IEEE_)
    return z;
  if (!finitef(z) && finitef(x)) {
    /* sinhf overflow */
    return (float)__kernel_standard((double)x, (double)x, 125);
  } else
    return z;
#endif
}

#endif
