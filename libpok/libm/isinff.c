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

/*
 * Written by J.T. Conklin <jtc@NetBSD.org>.
 * Public domain.
 */


/*
 * isinff(x) returns 1 is x is inf, else 0;
 * no branching!
 */

#ifdef POK_NEEDS_LIBMATH

#include <libm.h>
#include "math_private.h"

int
isinff(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	ix ^= 0x7f800000;
	return (ix == 0);
}

#endif
