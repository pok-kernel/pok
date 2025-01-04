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

/* s_logbf.c -- float version of s_logb.c.
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

#include "math_private.h"
#include <libm.h>

float logbf(float x) {
  int32_t ix;
  GET_FLOAT_WORD(ix, x);
  ix &= 0x7fffffff; /* high |x| */
  if (ix == 0)
    return (float)-1.0 / fabsf(x);
  if (ix >= 0x7f800000)
    return x * x;
  if ((ix >>= 23) == 0) /* IEEE 754 logb */
    return -126.0;
  else
    return (float)(ix - 127);
}

#endif
