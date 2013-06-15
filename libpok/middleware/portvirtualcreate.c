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
 * Created by julien on Tue Sep  8 07:39:08 2009 
 */


#include <core/dependencies.h>

#ifdef POK_NEEDS_PORTS_VIRTUAL

#include <types.h>
#include <errno.h>
#include <libc/stdio.h>
#include <core/syscall.h>
#include <middleware/port.h>

pok_ret_t pok_port_virtual_create (char* name, pok_port_id_t* id)
{
   return (pok_syscall2 (POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE, (uint32_t) name, (uint32_t) id));
}

#endif
