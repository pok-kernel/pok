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
 * \file    include/bsp.h
 * \brief   Interfaces that BSP must provide
 * \author  Julian Pidancet
 * \date    2008-2009
 */

#ifndef __POK_BSP_H__
#define __POK_BSP_H__

#include <types.h>
#include <errno.h>


pok_ret_t pok_bsp_init();

pok_ret_t pok_bsp_irq_acknowledge (uint8_t irq);

pok_ret_t pok_bsp_irq_register (uint8_t irq,
				                    void    (*handler)(void));
/* use only for the hardware interrupts */
#ifdef POK_NEEDS_X86_VMM
pok_ret_t pok_bsp_irq_register_hw (uint8_t   irq,
				   void      (*irq_handler)(unsigned, void*));
pok_ret_t pok_bsp_irq_unregister_hw (uint8_t   irq);

pok_ret_t pok_bsp_irq_register_vcpu(uint8_t vector,void (*handle_irq)(uint8_t));
pok_ret_t pok_bsp_irq_unregister_vcpu(uint8_t vector);

#endif /* POK_NEEDS_X86_VMM */

/* only used on x86, if the partition needs interrupt forwarding */
pok_ret_t pok_bsp_irq_partition_enable (uint8_t level);
pok_ret_t pok_bsp_irq_partition_disable (uint8_t level);
pok_ret_t pok_bsp_irq_partition_ack (unsigned vector);

void  *pok_bsp_mem_alloc (size_t size);

pok_ret_t pok_bsp_time_init ();

bool_t pok_cons_write (const char* s,
                       size_t length);

#ifdef POK_NEEDS_DEBUG
void pok_bsp_debug();
#endif

#endif /* !BSP_H_ */
