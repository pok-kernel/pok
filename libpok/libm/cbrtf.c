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

/* s_cbrtf.c -- float version of s_cbrt.c.
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

#include <types.h>
#include <libm.h>
#include "math_private.h"

/* cbrtf(x)
 * Return cube root of x
 */
static const unsigned
	B1 = 709958130, /* B1 = (84+2/3-0.03306235651)*2**23 */
	B2 = 642849266; /* B2 = (76+2/3-0.03306235651)*2**23 */

static const float
C =  5.4285717010e-01, /* 19/35     = 0x3f0af8b0 */
D = -7.0530611277e-01, /* -864/1225 = 0xbf348ef1 */
E =  1.4142856598e+00, /* 99/70     = 0x3fb50750 */
F =  1.6071428061e+00, /* 45/28     = 0x3fcdb6db */
G =  3.5714286566e-01; /* 5/14      = 0x3eb6db6e */

float
cbrtf(float x)
{
	float r,s,t;
	int32_t hx;
	uint32_t sign;
	uint32_t high;

	GET_FLOAT_WORD(hx,x);
	sign=hx&0x80000000; 		/* sign= sign(x) */
	hx  ^=sign;
	if(hx>=0x7f800000) return(x+x); /* cbrt(NaN,INF) is itself */
	if(hx==0)
	    return(x);		/* cbrt(0) is itself */

	SET_FLOAT_WORD(x,hx);	/* x <- |x| */
    /* rough cbrt to 5 bits */
	if(hx<0x00800000) 		/* subnormal number */
	  {SET_FLOAT_WORD(t,0x4b800000); /* set t= 2**24 */
	   t*=x; GET_FLOAT_WORD(high,t); SET_FLOAT_WORD(t,high/3+B2);
	  }
	else
	  SET_FLOAT_WORD(t,hx/3+B1);


    /* new cbrt to 23 bits */
	r=t*t/x;
	s=C+r*t;
	t*=G+F/(s+E+D/s);

    /* retore the sign bit */
	GET_FLOAT_WORD(high,t);
	SET_FLOAT_WORD(t,high|sign);
	return(t);
}

#endif

