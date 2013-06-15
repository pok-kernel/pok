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
 * Created by julien on Fri Aug 21 16:14:45 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_ERROR_HANDLING

#include <core/syscall.h>

void pok_error_raise_application_error (char* msg, uint32_t msg_size)
{
   pok_syscall2 (POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR, (uint32_t) msg, msg_size);
}

#endif
