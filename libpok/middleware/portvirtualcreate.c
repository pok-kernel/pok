/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_PORTS_VIRTUAL

#include <core/syscall.h>
#include <errno.h>
#include <libc/stdio.h>
#include <middleware/port.h>
#include <types.h>

pok_ret_t pok_port_virtual_create(char *name, pok_port_id_t *id) {
  return (pok_syscall2(POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE, (uint32_t)name,
                       (uint32_t)id));
}

#endif
