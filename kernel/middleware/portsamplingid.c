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

#ifdef POK_NEEDS_PORTS_SAMPLING
#include <core/sched.h>
#include <libc.h>
#include <middleware/port.h>
#include <types.h>

extern char *pok_ports_names[POK_CONFIG_NB_PORTS];
extern uint8_t pok_ports_kind[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_sampling_id(char *name, pok_port_id_t *id) {
  uint8_t i;

  for (i = 0; i < POK_CONFIG_NB_PORTS; i++) {
    if ((strcmp(name, pok_ports_names[i]) == 0) &&
        (pok_ports_kind[i] == POK_PORT_KIND_SAMPLING)) {
      if (!pok_own_port(POK_SCHED_CURRENT_PARTITION, i)) {
        return POK_ERRNO_PORT;
      }

      *id = i;

      return POK_ERRNO_OK;
    }
  }
  return POK_ERRNO_NOTFOUND;
}

#endif
