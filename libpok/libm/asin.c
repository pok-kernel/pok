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

/* @(#)w_asin.c 5.1 93/09/24 */
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
 * wrapper asin(x)
 */

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#ifdef __weak_alias
__weak_alias(asin, _asin)
#endif

    double asin(double x) /* wrapper asin */
{
#ifdef _IEEE_LIBM
  return __ieee754_asin(x);
#else
  double z;
  z = __ieee754_asin(x);
  return z;
  if (fabs(x) > 1.0) {
    return __kernel_standard(x, x, 2); /* asin(|x|>1) */
  } else
    return z;
#endif
}

#endif
