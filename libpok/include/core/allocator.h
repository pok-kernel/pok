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

#include <types.h>
#include <core/dependencies.h>

#ifdef POK_NEEDS_ALLOCATOR

/*
 * This file contains memory allocation functionnalities.
 * You can tweak/tune the memory allocator with the following macros:
 *  - POK_CONFIG_ALLOCATOR_NB_SPACES : the number of memory spaces
 *    that can be allocated. It can corresponds to the successive
 *    call of malloc() or calloc() or pok_allocator_allocate()
 *  - POK_CONFIG_ALLOCATOR_MEMORY_SIZE : the amount of memory
 *    the allocator can allocate
 */

void* pok_allocator_allocate (size_t needed_size);
/*
 * This function allocates memory. The argument is the amount
 * of memory the user needs. This function is called by libc
 * functions malloc() and calloc()
 */

void pok_allocator_free (void* ptr);
/*
 * This function frees memory. The argument is a previously
 * allocated memory chunk. Be careful, the time required
 * to free the memory is indeterministic, you should not
 * free memory if your program has strong timing requirements.
 */

#endif

