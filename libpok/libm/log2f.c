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

/* w_log10f.c -- float version of w_log10.c.
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
 * wrapper log2f(X)
 */

#include <libm.h>
#include "math_private.h"


float
log2f(float x)		/* wrapper log2f */
{
#ifdef _IEEE_LIBM
	return __ieee754_log2f(x);
#else
	float z;
	z = __ieee754_log2f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<=(float)0.0) {
	    if(x==(float)0.0)
	        /* log2(0) */
	        return (float)__kernel_standard((double)x,(double)x,148);
	    else
	        /* log2(x<0) */
	        return (float)__kernel_standard((double)x,(double)x,149);
	} else
	    return z;
#endif
}

#endif
