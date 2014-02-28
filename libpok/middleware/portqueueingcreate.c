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

#include <core/dependencies.h>

#ifdef POK_NEEDS_PORTS_QUEUEING

#include <types.h>
#include <core/syscall.h>
#include <middleware/port.h>

pok_ret_t pok_port_queueing_create (char*                                  name, 
                                    const pok_port_size_t                  size, 
                                    const pok_port_direction_t             direction,
                                    const pok_port_queueing_discipline_t   discipline,
                                    pok_port_id_t*                         id)
{
   return (pok_syscall5 (POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE,
                         (uint32_t) name, 
                         (uint32_t) size, 
                         (uint32_t) direction,
                         (uint32_t) discipline,
                         (uint32_t) id));
}

#endif
