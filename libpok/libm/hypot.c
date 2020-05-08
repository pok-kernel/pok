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
 *                                      Copyright (c) 2007-2020 POK team
 */

/* @(#)w_hypot.c 5.1 93/09/24 */
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
 * wrapper hypot(x,y)
 */

#include "math_private.h"
#include "namespace.h"
#include <libm.h>

#ifdef __weak_alias
__weak_alias(hypot, _hypot)
#endif

    double hypot(double x, double y) /* wrapper hypot */
{
#ifdef _IEEE_LIBM
  return __ieee754_hypot(x, y);
#else
  double z;
  z = __ieee754_hypot(x, y);
  if (_LIB_VERSION == _IEEE_)
    return z;
  if ((!finite(z)) && finite(x) && finite(y))
    return __kernel_standard(x, y, 4); /* hypot overflow */
  else
    return z;
#endif
}

#endif
