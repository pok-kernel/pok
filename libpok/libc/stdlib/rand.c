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
 * Created by julien on Sat Jan 31 20:12:07 2009 
 */

#include <core/dependencies.h>

#ifdef POK_CONFIG_NEEDS_FUNC_RAND

#include <types.h>
#include <libc/stdlib.h>

static uint32_t next = 1;

int rand()
{
	/* LINTED integer overflow */
	return (int)((next = next * 1103515245 + 12345) % ((uint32_t)RAND_MAX + 1));
}
#endif
