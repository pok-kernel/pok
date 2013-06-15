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

#ifdef POK_NEEDS_PORTS_VIRTUAL

#include <errno.h>
#include <types.h>
#include <core/sched.h>
#include <core/lockobj.h>
#include <middleware/port.h>
#include <middleware/queue.h>
#include <libc.h>

extern uint8_t          pok_ports_nb_destinations[POK_CONFIG_NB_PORTS];              /**  from deployment.c when using code generation */
extern uint8_t          pok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS];   /**  from deployment.c when using code generation */
extern uint8_t*         pok_ports_destinations[POK_CONFIG_NB_PORTS];                 /**  from deployment.c when using code generation */
extern uint8_t          pok_ports_kind[POK_CONFIG_NB_PORTS];
extern uint8_t          pok_local_ports_to_global_ports[POK_CONFIG_NB_PORTS];

pok_ret_t pok_port_virtual_get_global (const pok_port_id_t local, pok_port_id_t* global)
{
   if (local >= POK_CONFIG_NB_PORTS)
   {
      return POK_ERRNO_PORT;
   }

   if (! pok_own_port (POK_SCHED_CURRENT_PARTITION, local))
   {
      return POK_ERRNO_PORT;
   }

   if (pok_ports_kind[local] != POK_PORT_KIND_VIRTUAL)
   {
      return POK_ERRNO_PORT;
   }

   *global = (pok_port_id_t) pok_local_ports_to_global_ports[local];

   return POK_ERRNO_OK;
}

#endif
