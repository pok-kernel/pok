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

/**
 * \file    middleware/portvirtualutils.c
 * \date    2008-2019
 * \brief   Various functions for virtual ports management.
 * \author  Etienne Borde
 */

#include <core/syscall.h>
#include <errno.h>
#include <middleware/port.h>
#include <types.h>

#ifdef POK_NEEDS_PORTS_VIRTUAL

pok_ret_t pok_port_virtual_node(uint32_t port, uint8_t *node) {
  return pok_syscall2(POK_SYSCALL_MIDDLEWARE_VIRTUAL_NODE, (uint32_t)port,
                      (uint32_t)node);
}

#endif

#ifdef POK_NEEDS_MAC_ADDR

pok_ret_t pok_node_mac_addr(uint8_t *node_id, char *addr) {
  return pok_syscall2(POK_SYSCALL_MIDDLEWARE_NODE_MAC_ADDR, (uint32_t)node_id,
                      (uint32_t)addr);
}

#endif
