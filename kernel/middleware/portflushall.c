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
 * \file    middleware/portflushall.c
 * \brief   Flush the ports and send the data of IN ports to OUT ports
 * \date    2008-2009
 * \author  Julien Delange
 * \author  Laurent Lec
 */

#if defined (POK_NEEDS_PORTS_QUEUEING) || defined (POK_NEEDS_PORTS_SAMPLING)

#include <types.h>
#include <libc.h>

#include <core/partition.h>
#include <core/lockobj.h>

#include <middleware/port.h>
#include <middleware/queue.h>

extern uint8_t          pok_ports_nb_destinations[POK_CONFIG_NB_PORTS];              /**  from deployment.c when using code generation */
extern uint8_t          pok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS];   /**  from deployment.c when using code generation */
extern uint8_t*         pok_ports_destinations[POK_CONFIG_NB_PORTS];                 /**  from deployment.c when using code generation */
extern uint8_t*         pok_ports_by_partition[POK_CONFIG_NB_PARTITIONS];            /**  from deployment.c when using code generation */
extern pok_port_t       pok_ports[POK_CONFIG_NB_PORTS];                              /**  from deployment.c when using code generation */
extern uint8_t		      pok_ports_nodes[POK_CONFIG_NB_GLOBAL_PORTS];                 /**  from deployment.c when using code generation */
extern uint8_t          pok_global_ports_to_local_ports[POK_CONFIG_NB_GLOBAL_PORTS]; /**  from deployment.c when using code generation */
extern pok_queue_t      pok_queue;

uint8_t                 pok_buffer_flush[POK_PORT_MAX_SIZE];

void pok_port_flush_partition (uint8_t pid)
{
   uint8_t nb;
   uint8_t local;
   uint8_t i;
   uint8_t j;
   uint8_t ndest;
   uint8_t local_dest; /* recipient port, global id */
   uint8_t global_dest; /* recipient port, local id  */
   pok_port_size_t len;

   nb = pok_ports_nb_ports_by_partition[pid];

   for (i = 0 ; i < nb ; i++)
   {
      local = pok_ports_by_partition[pid][i];

      if (pok_ports[local].direction != POK_PORT_DIRECTION_OUT)
      {
         continue;
      }

      if (pok_ports[local].empty == TRUE)  
      {
         continue;
      }

      if (pok_ports[local].must_be_flushed == FALSE)
      {
         continue;
      }

      len = pok_port_consumed_size (local);

      if (pok_port_get (local, pok_buffer_flush, len) != POK_ERRNO_OK)
      {
         continue;
      }

      ndest = pok_ports_nb_destinations[local];

      for (j=0 ; j < ndest ; j++)
      {
         global_dest = pok_ports_destinations[local][j];

         local_dest = pok_global_ports_to_local_ports[global_dest];
         if (pok_ports[local_dest].ready != TRUE)
         {
            continue;
         }

         if (pok_ports[local_dest].direction != POK_PORT_DIRECTION_IN)
         {
            continue;
         }

         pok_port_write (local_dest, pok_buffer_flush, len);

         pok_lockobj_eventbroadcast (&pok_ports[local_dest].lock);
         /*
          * We notify every waiting thread in this port that data are
          * now avaiable
          */
      }
      pok_lockobj_eventbroadcast (&pok_ports[local].lock);
      /*
       * We notify every thread blocked on this port that free space
       * is now available
       */

      pok_ports[local].must_be_flushed = FALSE;
   }
}

/**
 * Flush all the ports, write all OUT ports to their destinations.
 * This function is called at each major frame
 */
void pok_port_flushall (void)
{
   uint8_t p;
   for (p = 0 ; p < POK_CONFIG_NB_PARTITIONS ; p++)
   {
      if ((pok_partitions[p].mode == POK_PARTITION_MODE_NORMAL) || (pok_partitions[p].mode == POK_PARTITION_MODE_IDLE))
      {
         pok_port_flush_partition (p);
      }
   }
}

#endif
