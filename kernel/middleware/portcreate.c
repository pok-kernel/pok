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

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)

#include <errno.h>
#include <types.h>
#include <core/error.h>
#include <core/partition.h>
#include <middleware/port.h>
#include <middleware/queue.h>
#include <libc.h>

extern uint8_t       pok_current_partition;
extern uint8_t       pok_ports_parts_nb_ports[POK_CONFIG_NB_PARTITIONS];
extern uint8_t*      pok_ports_parts_ports_identifiers[POK_CONFIG_NB_PARTITIONS];
extern pok_port_t    pok_ports[POK_CONFIG_NB_PORTS];
extern pok_queue_t   pok_queue;

pok_ret_t pok_port_create (char* name,
													 const pok_port_size_t size,
													 const pok_port_direction_t direction,
													 uint8_t kind, pok_port_id_t* id)
{
	 uint8_t   gid;
	 pok_ret_t ret;

	 ret = POK_ERRNO_OK;


	 if (size > POK_PORT_MAX_SIZE)
	 {
#if (defined POK_NEEDS_PARTITIONS) && (defined POK_NEEDS_ERROR_HANDLING)
			POK_ERROR_CURRENT_PARTITION(POK_ERROR_KIND_PARTITION_CONFIGURATION);
#endif
			return POK_ERRNO_PORT;
	 }

	 if (size > pok_queue.available_size)
	 {
#if (defined POK_NEEDS_PARTITIONS) && (defined POK_NEEDS_ERROR_HANDLING)
			POK_ERROR_CURRENT_PARTITION(POK_ERROR_KIND_PARTITION_CONFIGURATION);
#endif
			return POK_ERRNO_PORT;
	 }

	 if ((direction != POK_PORT_DIRECTION_IN) &&
			 (direction != POK_PORT_DIRECTION_OUT))
	 {
#if (defined POK_NEEDS_PARTITIONS) && (defined POK_NEEDS_ERROR_HANDLING)
			POK_ERROR_CURRENT_PARTITION(POK_ERROR_KIND_PARTITION_CONFIGURATION);
#endif
			return POK_ERRNO_PORT;
	 }

	 switch (kind)
	 {
			case POK_PORT_KIND_SAMPLING:
				 {
#ifdef POK_NEEDS_PORTS_SAMPLING
						ret = pok_port_sampling_id (name, &gid);
#endif
						break;
				 }
			case POK_PORT_KIND_QUEUEING:
				 {
#ifdef POK_NEEDS_PORTS_QUEUEING
						ret = pok_port_queueing_id (name, &gid);
#endif
						break;
				 }
			default:
				 {
						return POK_ERRNO_EINVAL;
						break;
				 }
	 }

	 if (ret != POK_ERRNO_OK)
	 {
#if (defined POK_NEEDS_PARTITIONS) && (defined POK_NEEDS_ERROR_HANDLING)
			POK_ERROR_CURRENT_PARTITION(POK_ERROR_KIND_PARTITION_INIT);
#endif
			return ret;
	 }

	 if (! pok_own_port (POK_SCHED_CURRENT_PARTITION, gid))
	 {
			return POK_ERRNO_PORT;
	 }

	 /*
		* Check if the port was already created
		* If it's already created, we return ERRNO_EXISTS but indicate
		* the right port-id. This should not be taken as an assumption
		* but could help the developper when a partition is restarted.
		*/
	 if (pok_ports[gid].ready == TRUE)
	 {
			*id = gid;
			return POK_ERRNO_EXISTS;
	 }

	 pok_ports[gid].index      = pok_queue.size - pok_queue.available_size;
	 pok_ports[gid].off_b      = 0;
	 pok_ports[gid].off_e      = 0;
	 pok_ports[gid].size       = size;
	 pok_ports[gid].full       = FALSE;
	 pok_ports[gid].partition  = pok_current_partition;
	 pok_ports[gid].direction  = direction;
	 pok_ports[gid].ready      = TRUE;
	 pok_ports[gid].kind       = kind;

	 pok_queue.available_size  = pok_queue.available_size - size;

	 *id = gid;

	 return POK_ERRNO_OK;
}
#endif
