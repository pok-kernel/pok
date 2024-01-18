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

/* w_asinf.c -- float version of w_asin.c.
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
 * wrapper asinf(x)
 */

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#ifdef __weak_alias
__weak_alias(asinf, _asinf)
#endif

    float asinf(float x) /* wrapper asinf */
{
#ifdef _IEEE_LIBM
  return __ieee754_asinf(x);
#else
  float z;
  z = __ieee754_asinf(x);
  if (_LIB_VERSION == _IEEE_ || isnanf(x))
    return z;
  if (fabsf(x) > (float)1.0) {
    /* asinf(|x|>1) */
    return (float)__kernel_standard((double)x, (double)x, 102);
  } else
    return z;
#endif
}

#endif
