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
 *                                      Copyright (c) 2007-2022 POK team
 */

/* s_ldexp0f.c -- float version of s_ldexp0.c.
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
#include <errno.h>
#include <libm.h>

float ldexpf(float value, int exp0) {
  if (!finitef(value) || value == (float)0.0)
    return value;
  value = scalbnf(value, exp0);
  /*	if(!finitef(value)||value==(float)0.0) errno = POK_ERRNO_ERANGE;
   */
  return value;
}

#endif
