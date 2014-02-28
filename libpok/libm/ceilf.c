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

/* s_ceilf.c -- float version of s_ceil.c.
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
#include <libm.h>
#include "math_private.h"

static const float huge = 1.0e30;

float
ceilf(float x)
{
	int32_t i0,jj0;
	uint32_t i;

	GET_FLOAT_WORD(i0,x);
	jj0 = ((i0>>23)&0xff)-0x7f;
	if(jj0<23) {
	    if(jj0<0) { 	/* raise inexact if x != 0 */
		if(huge+x>(float)0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0<0) {i0=0x80000000;}
		    else if(i0!=0) { i0=0x3f800000;}
		}
	    } else {
		i = (0x007fffff)>>jj0;
		if((i0&i)==0) return x; /* x is integral */
		if(huge+x>(float)0.0) {	/* raise inexact flag */
		    if(i0>0) i0 += (0x00800000)>>jj0;
		    i0 &= (~i);
		}
	    }
	} else {
	    if(jj0==0x80) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	}
	SET_FLOAT_WORD(x,i0);
	return x;
}

#endif
