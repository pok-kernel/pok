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

/* s_nextafterf.c -- float version of s_nextafter.c.
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

float nextafterf(float x, float y) {
  int32_t hx, hy, ix, iy;

  GET_FLOAT_WORD(hx, x);
  GET_FLOAT_WORD(hy, y);
  ix = hx & 0x7fffffff; /* |x| */
  iy = hy & 0x7fffffff; /* |y| */

  if ((ix > 0x7f800000) || /* x is nan */
      (iy > 0x7f800000))   /* y is nan */
    return x + y;
  if (x == y)
    return x;                                 /* x=y, return x */
  if (ix == 0) {                              /* x == 0 */
    SET_FLOAT_WORD(x, (hy & 0x80000000) | 1); /* return +-minsubnormal */
    y = x * x;
    if (y == x)
      return y;
    else
      return x; /* raise underflow flag */
  }
  if (hx >= 0) {   /* x > 0 */
    if (hx > hy) { /* x > y, x -= ulp */
      hx -= 1;
    } else { /* x < y, x += ulp */
      hx += 1;
    }
  } else {                    /* x < 0 */
    if (hy >= 0 || hx > hy) { /* x < y, x -= ulp */
      hx -= 1;
    } else { /* x > y, x += ulp */
      hx += 1;
    }
  }
  hy = hx & 0x7f800000;
  if (hy >= 0x7f800000)
    return x + x;        /* overflow  */
  if (hy < 0x00800000) { /* underflow */
    y = x * x;
    if (y != x) { /* raise underflow flag */
      SET_FLOAT_WORD(y, hx);
      return y;
    }
  }
  SET_FLOAT_WORD(x, hx);
  return x;
}

#endif
