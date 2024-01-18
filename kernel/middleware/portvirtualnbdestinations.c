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

#ifdef POK_NEEDS_PORTS_VIRTUAL

#include <core/lockobj.h>
#include <core/sched.h>
#include <errno.h>
#include <libc.h>
#include <middleware/port.h>
#include <middleware/queue.h>
#include <types.h>

extern uint8_t
    pok_ports_nb_destinations[POK_CONFIG_NB_PORTS]; /**  from deployment.c when
                                                       using code generation */
extern uint8_t pok_ports_nb_ports_by_partition
    [POK_CONFIG_NB_PARTITIONS]; /**  from deployment.c when using code
                                   generation */
extern uint8_t pok_ports_kind[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_virtual_nb_destinations(const pok_port_id_t id,
                                           uint32_t *result) {
  if (id >= POK_CONFIG_NB_PORTS) {
    return POK_ERRNO_PORT;
  }

  if (!pok_own_port(POK_SCHED_CURRENT_PARTITION, id)) {
    return POK_ERRNO_PORT;
  }

  if (pok_ports_kind[id] != POK_PORT_KIND_VIRTUAL) {
    return POK_ERRNO_PORT;
  }

  *result = pok_ports_nb_destinations[id];

  return POK_ERRNO_OK;
}

#endif
