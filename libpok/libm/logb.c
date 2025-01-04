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

/* @(#)s_logb.c 5.1 93/09/24 */
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
 * double logb(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#ifdef POK_NEEDS_LIBMATH

#include "math_private.h"
#include <libm.h>

double logb(double x) {
  int32_t lx, ix;
  EXTRACT_WORDS(ix, lx, x);
  ix &= 0x7fffffff; /* high |x| */
  if ((ix | lx) == 0)
    return -1.0 / fabs(x);
  if (ix >= 0x7ff00000)
    return x * x;
  if ((ix >>= 20) == 0) /* IEEE 754 logb */
    return -1022.0;
  else
    return (double)(ix - 1023);
}

#endif
