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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#ifdef POK_NEEDS_DEBUG

#ifndef __POK_KERNEL_DEBUG_H_
#define __POK_KERNEL_DEBUG_H_

#include <errno.h>
#include <types.h>
#include <libc.h>

void		pok_fatal(const char* message);

int      debug_strlen (const char* str);

#define POK_FATAL(arg) pok_fatal(arg)

#define POK_DEBUG_PRINT_CURRENT_STATE pok_debug_print_current_state();

#endif /* __POK_KERNELDEBUG_H__ */
#else
   #define POK_DEBUG_PRINT_CURRENT_STATE
   #define POK_FATAL(arg) 
#endif /* POK_NEEDS_DEBUG */

