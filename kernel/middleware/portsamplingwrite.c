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

/**
 * \file    middleware/portsamplingwrite.c
 * \author  Julien Delange
 * \date    2008-2009
 * \brief   Send data on a sampling port
 */

#ifdef POK_NEEDS_PORTS_SAMPLING
#include <errno.h>
#include <types.h>
#include <libc.h>
#include <core/debug.h>
#include <core/sched.h>
#include <core/lockobj.h>
#include <middleware/port.h>
#include <middleware/queue.h>

extern uint8_t       pok_current_partition;
extern uint8_t       pok_ports_parts_nb_ports[POK_CONFIG_NB_PARTITIONS];
extern uint8_t*      pok_ports_parts_ports_identifiers[POK_CONFIG_NB_PARTITIONS];
extern uint8_t*      pok_ports_local_to_global[POK_CONFIG_NB_PARTITIONS];
extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];
extern pok_queue_t   pok_queues[POK_CONFIG_NB_PARTITIONS];


pok_ret_t pok_port_sampling_write (const pok_port_id_t   id, 
                                  const void*            data, 
                                  const pok_port_size_t  len)
{
   /* We don't handle the timeout at this time */

   if (data == NULL)
   {
      return POK_ERRNO_EINVAL;
   }

   if (id > POK_CONFIG_NB_PORTS)
   {
      return POK_ERRNO_EINVAL;
   }

   if (! pok_own_port (POK_SCHED_CURRENT_PARTITION, id))
   {
      return POK_ERRNO_PORT;
   }

   if (pok_ports[id].ready != TRUE)
   {
      return POK_ERRNO_EINVAL;
   }
   
   if (len > pok_ports[id].size)
   {
      return POK_ERRNO_EINVAL;
   }

   if (pok_ports[id].direction != POK_PORT_DIRECTION_OUT)
   {
      return POK_ERRNO_DIRECTION;
   }

   pok_lockobj_lock (&pok_ports[id].lock, NULL);

   pok_port_write (id, data, len);
   
   pok_lockobj_unlock (&pok_ports[id].lock, NULL);

   pok_ports[id].must_be_flushed = TRUE;

   return POK_ERRNO_OK;
}

#endif
