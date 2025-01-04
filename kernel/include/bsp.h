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
 * \file    include/bsp.h
 * \brief   Interfaces that BSP must provide
 * \author  Julian Pidancet
 * \date    2008-2009
 */

#ifndef __POK_BSP_H__
#define __POK_BSP_H__

#include <errno.h>
#include <types.h>

pok_ret_t pok_bsp_init();

pok_ret_t pok_bsp_irq_acknowledge(uint8_t irq);

pok_ret_t pok_bsp_irq_register(uint8_t irq, void (*handler)(void));

void *pok_bsp_mem_alloc(size_t size);

pok_ret_t pok_bsp_time_init();

bool_t pok_cons_write(const char *s, size_t length);

void pok_cons_get_char(char *c);

#ifdef POK_NEEDS_DEBUG
void pok_bsp_debug();
#endif

#endif /* !BSP_H_ */
