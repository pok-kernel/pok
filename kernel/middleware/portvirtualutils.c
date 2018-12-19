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
 *                                      Copyright (c) 2007-2018 POK team 
 *
 * Created by etienne on Su Dec 16 19:25:21 2018
 */

/**
 * \file    middleware/portvirtualutils.c
 * \date    2008-2019
 * \brief   Various functions for virtual ports management.
 * \author  Etienne Borde
 */ 

#include <middleware/port.h>

#ifdef POK_NEEDS_PORTS_VIRTUAL

extern uint8_t    pok_ports_nodes[POK_CONFIG_NB_PORTS];
extern char *     pok_nodes_addr[POK_CONFIG_NB_NODES];

pok_ret_t pok_port_virtual_node(uint32_t port, uint8_t * node)
{
  *node = pok_ports_nodes[port];
  return POK_ERRNO_OK;
}

#ifdef POK_NEEDS_MAC_ADDR
pok_ret_t pok_node_mac_addr(uint8_t * node_id, char * addr)
{
  char * node_addr = pok_nodes_addr[*node_id];
  int i;
  for(i=0; i<6; i++)
  {
    addr[i] = node_addr[i];
  }
  return POK_ERRNO_OK;
}
#endif

#endif

