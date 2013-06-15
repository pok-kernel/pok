/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Fri Jan 30 14:41:34 2009 
 */

/* w_sqrtf.c -- float version of w_sqrt.c.
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
 * wrapper sqrtf(x)
 */

#include "math_private.h"
#include <libm.h>

float
sqrtf(float x)		/* wrapper sqrtf */
{
#ifdef _IEEE_LIBM
	return __ieee754_sqrtf(x);
#else
	float z;
	z = __ieee754_sqrtf(x);
	if (isnanf(x)) return z;
	if(x<(float)0.0) {
	    /* sqrtf(negative) */
	    return (float)-1.0;
	} else
	    return z;
#endif
}

#endif
