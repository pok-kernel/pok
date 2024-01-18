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
 *                                      Copyright (c) 2007-2024 POK team
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_PORTS_QUEUEING
#include <core/syscall.h>
#include <middleware/port.h>
#include <types.h>

pok_ret_t pok_port_queueing_send(const pok_port_id_t id, const void *data,
                                 const pok_port_size_t len,
                                 const uint64_t timeout) {
  return (pok_syscall4(POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND, (uint32_t)id,
                       (uint32_t)data, (uint32_t)len, (uint32_t)timeout));
}

#endif
