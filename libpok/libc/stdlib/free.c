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
 * Created by julien on Thu Jul 30 15:49:30 2009 
 */

#include <core/dependencies.h>

#ifdef POK_CONFIG_NEEDS_FUNC_FREE

#include <types.h>
#include <core/allocator.h>

void free(void* ptr)
{
   pok_allocator_free (ptr);

	return;
}
#endif

