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

/* @(#)w_log.c 5.1 93/09/24 */
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
 * wrapper log(x)
 */

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#ifdef __weak_alias
__weak_alias(log, _log)
#endif

    double log(double x) /* wrapper log */
{
#ifdef _IEEE_LIBM
  return __ieee754_log(x);
#else
  double z;
  z = __ieee754_log(x);
  if (_LIB_VERSION == _IEEE_ || isnan(x) || x > 0.0)
    return z;
  if (x == 0.0)
    return __kernel_standard(x, x, 16); /* log(0) */
  else
    return __kernel_standard(x, x, 17); /* log(x<0) */
#endif
}

#endif
