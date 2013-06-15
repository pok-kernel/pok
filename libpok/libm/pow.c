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



/* @(#)w_pow.c 5.2 93/10/01 */
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
 * wrapper pow(x,y) return x**y
 */

#include <libm.h>
#include "math_private.h"


double
pow(double x, double y)	/* wrapper pow */
{
#ifdef _IEEE_LIBM
	return  __ieee754_pow(x,y);
#else
	double z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) return z;
	if(isnan(x)) {
	    if(y==0.0)
	        return __kernel_standard(x,y,42); /* pow(NaN,0.0) */
	    else
		return z;
	}
	if(x==0.0){
	    if(y==0.0)
	        return __kernel_standard(x,y,20); /* pow(0.0,0.0) */
	    if(finite(y)&&y<0.0)
	        return __kernel_standard(x,y,23); /* pow(0.0,negative) */
	    return z;
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z))
	            return __kernel_standard(x,y,24); /* pow neg**non-int */
	        else
	            return __kernel_standard(x,y,21); /* pow overflow */
	    }
	}
	if(z==0.0&&finite(x)&&finite(y))
	    return __kernel_standard(x,y,22); /* pow underflow */
	return z;
#endif
}

#endif
