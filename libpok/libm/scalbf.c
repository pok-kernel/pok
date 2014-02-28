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

/* w_scalbf.c -- float version of w_scalb.c.
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
 * wrapper scalbf(float x, float fn) is provide for
 * passing various standard test suite. One
 * should use scalbn() instead.
 */

#include <libm.h>
#include "math_private.h"

#include <errno.h>

#ifdef _SCALB_INT
float
scalbf(float x, int fn)		/* wrapper scalbf */
#else
float
scalbf(float x, float fn)		/* wrapper scalbf */
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_scalbf(x,fn);
#else
	float z;
	z = __ieee754_scalbf(x,fn);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!(finitef(z)||isnanf(z))&&finitef(x)) {
	    /* scalbf overflow */
	    return (float)__kernel_standard((double)x,(double)fn,132);
	}
	if(z==(float)0.0&&z!=x) {
	    /* scalbf underflow */
	    return (float)__kernel_standard((double)x,(double)fn,133);
	}
#ifndef _SCALB_INT
	if(!finitef(fn)) errno = POK_ERRNO_ERANGE;
#endif
	return z;
#endif
}

#endif
