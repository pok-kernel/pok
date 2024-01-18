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

#ifdef POK_NEEDS_PORTS_SAMPLING

#include <core/lockobj.h>
#include <errno.h>
#include <middleware/port.h>
#include <middleware/queue.h>
#include <types.h>

extern pok_port_t pok_ports[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_sampling_create(char *name, const pok_port_size_t size,
                                   const pok_port_direction_t direction,
                                   const uint64_t refresh, pok_port_id_t *id) {
  pok_ret_t ret;
  pok_lockobj_attr_t lockattr;

  ret = pok_port_create(name, size, direction, POK_PORT_KIND_SAMPLING, id);

  if (ret != POK_ERRNO_OK) {
    return ret;
  }

  pok_ports[*id].refresh = refresh;
  pok_ports[*id].last_receive = 0;

  lockattr.kind = POK_LOCKOBJ_KIND_EVENT;
  lockattr.locking_policy = POK_LOCKOBJ_POLICY_STANDARD;
  ret = pok_lockobj_create(&pok_ports[*id].lock, &lockattr);

  return ret;
}

#endif
