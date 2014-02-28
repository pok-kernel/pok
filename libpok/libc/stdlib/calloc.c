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

#ifdef POK_CONFIG_NEEDS_FUNC_CALLOC

#include <types.h>
#include <core/allocator.h>


void *calloc(size_t count, size_t size)
{
   return pok_allocator_allocate (count * size);
}
#endif
