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
 * Created by julien on Fri Jan 23 15:28:31 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_ERROR_HANDLING
#include <types.h>
#include <core/error.h>

#ifdef POK_NEEDS_DEBUG
#include <libc/stdio.h>
#endif

void pok_error_confirm (const uint32_t error_id, const uint32_t thread_id)
{
   pok_error_log (error_id, thread_id);
   return;
}

#endif
