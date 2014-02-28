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

/* w_expf.c -- float version of w_exp.c.
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
 * wrapper expf(x)
 */

#include <libm.h>
#include "namespace.h"
#include "math_private.h"

#ifdef __weak_alias
__weak_alias(expf, _expf)
#endif

static const float
o_threshold=  8.8721679688e+01,  /* 0x42b17180 */
u_threshold= -1.0397208405e+02;  /* 0xc2cff1b5 */

float
expf(float x)		/* wrapper expf */
{
#ifdef _IEEE_LIBM
	return __ieee754_expf(x);
#else
	float z;
	z = __ieee754_expf(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finitef(x)) {
	    if(x>o_threshold)
	        /* exp overflow */
	        return (float)__kernel_standard((double)x,(double)x,106);
	    else if(x<u_threshold)
	        /* exp underflow */
	        return (float)__kernel_standard((double)x,(double)x,107);
	}
	return z;
#endif
}

#endif
