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
 * \file    include/middleware/port.h
 * \date    2008-2009
 * \author  Julien Delange
 * \brief   Describe queueing and sampling ports structures
 */

#ifndef __POK_KERNEL_PORTS_H__
#define __POK_KERNEL_PORTS_H__

#include <types.h>
#include <errno.h>
#include <core/lockobj.h>

#define POK_PORT_MAX_SIZE 512

typedef enum
{
	 POK_PORT_QUEUEING_DISCIPLINE_FIFO      = 1,
	 POK_PORT_QUEUEING_DISCIPLINE_PRIORITY  = 2
} pok_port_queueing_disciplines_t;

typedef enum
{
	 POK_PORT_DIRECTION_IN   = 1,
	 POK_PORT_DIRECTION_OUT  = 2
} pok_port_directions_t;

typedef pok_queueing_discipline_t pok_port_queueing_discipline_t;

typedef enum
{
	 POK_PORT_KIND_QUEUEING  = 1,
	 POK_PORT_KIND_SAMPLING  = 2,
#ifdef POK_NEEDS_PORTS_VIRTUAL
	 POK_PORT_KIND_VIRTUAL   = 2,
#endif
	 POK_PORT_KIND_INVALID   = 10
} pok_port_kinds_t;

typedef struct
{
	 pok_port_id_t                    identifier;
	 pok_partition_id_t               partition;
	 pok_port_size_t                  index;
	 bool_t                           full;
	 pok_port_size_t                  size;
	 pok_port_size_t                  off_b; /* Offset of the beginning of the buffer */
	 pok_port_size_t                  off_e; /* Offset of the end of the buffer */
	 pok_port_direction_t             direction;
	 pok_port_queueing_discipline_t   discipline;
	 pok_bool_t                       ready;
	 bool_t                           empty;
	 uint8_t                          kind;
	 uint64_t                         refresh;
	 uint64_t                         last_receive;
	 pok_lockobj_t                    lock;
	 bool_t                           must_be_flushed;
}pok_port_t;

#ifdef POK_NEEDS_PORTS_QUEUEING
/* Queueing port functions */
typedef struct
{
	 pok_port_size_t      size;
	 pok_port_direction_t direction;
	 uint8_t              waiting_processes;
}pok_port_queueing_status_t;


pok_ret_t pok_port_queueing_create (char*                                     name,
																		const pok_port_size_t                     size,
																		const pok_port_direction_t                direction,
																		const pok_port_queueing_discipline_t      discipline,
																		pok_port_id_t*                            id);

pok_ret_t pok_port_queueing_receive (const pok_port_id_t                      id,
																		 uint64_t                                 timeout,
																		 const pok_port_size_t                    maxlen,
																		 void*                                    data,
																		 pok_port_size_t*                         len);

pok_ret_t pok_port_queueing_send (const pok_port_id_t                         id,
																	const void*                                 data,
																	const pok_port_size_t                       len,
																	uint64_t                              timeout);

pok_ret_t pok_port_queueing_status (const pok_port_id_t                    id,
																		pok_port_queueing_status_t*      status);

pok_ret_t pok_port_queueing_id     (char*                                     name,
																		pok_port_id_t*                            id);
#endif

#ifdef POK_NEEDS_PORTS_SAMPLING
/* Sampling port functions */

typedef struct
{
	 pok_port_size_t      size;
	 pok_port_direction_t direction;
	 uint64_t             refresh;
	 bool_t               validity;
}pok_port_sampling_status_t;


pok_ret_t pok_port_sampling_create (char*                                     name,
																		const pok_port_size_t                     size,
																		const pok_port_direction_t                direction,
																		const uint64_t                            refresh,
																		pok_port_id_t*                            id);

pok_ret_t pok_port_sampling_write (const pok_port_id_t                        id,
																	 const void*                                data,
																	 const pok_port_size_t                      len);

pok_ret_t pok_port_sampling_read (const pok_port_id_t                      id,
																	void*                                    message,
																	pok_port_size_t*                         len,
																	bool_t*                                  valid);

pok_ret_t pok_port_sampling_id     (char*                                     name,
																		pok_port_id_t*                            id);

pok_ret_t pok_port_sampling_status (const pok_port_id_t                       id,
																		pok_port_sampling_status_t*               status);
#endif

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING)
/* Generic functions */
pok_ret_t pok_port_create (char*                                  name,
													 const pok_port_size_t                  size,
													 const pok_port_direction_t             direction,
													 uint8_t                                kind,
													 pok_port_id_t*                         id);

pok_ret_t pok_port_transfer (const uint8_t gid_dst,
														 const uint8_t gid_src);

void              pok_port_init(void);
void              pok_port_queueing_flushall (void);
uint8_t           pok_port_lid_to_gid (uint8_t lid);
pok_port_size_t   pok_port_available_size (uint8_t gid);
pok_port_size_t   pok_port_consumed_size (uint8_t gid);
pok_ret_t         pok_port_get (const uint32_t gid, void *data, const pok_port_size_t size);
pok_ret_t         pok_port_write (const uint8_t gid, const void *data, const pok_port_size_t size);
#endif

#if defined (POK_NEEDS_PORTS_SAMPLING) || defined (POK_NEEDS_PORTS_QUEUEING) ||defined (POK_NEEDS_PORTS_VIRTUAL)
bool_t            pok_own_port (const uint8_t partition, const uint8_t port);
#endif

#ifdef POK_NEEDS_PORTS_VIRTUAL
pok_ret_t pok_port_virtual_id (char*            name,
															 pok_port_id_t*   id);

pok_ret_t pok_port_virtual_nb_destinations (const pok_port_id_t id, uint32_t* result);
pok_ret_t pok_port_virtual_destination (const pok_port_id_t id, const uint32_t n, uint32_t* result);
pok_ret_t pok_port_virtual_get_global (const pok_port_id_t local, pok_port_id_t* global);
pok_ret_t pok_port_virtual_node(uint32_t port, uint8_t * node);
#ifdef POK_NEEDS_MAC_ADDR
pok_ret_t pok_node_mac_addr(uint8_t * node_id, char * addr);
#endif

#endif

#endif
