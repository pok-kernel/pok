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
 * @file
 * @author Fabien Chouteau
 * @brief  Leon3 IRQ management.
 */

#ifndef __POK_SPARC_LEON3_IRQ_H__
# define __POK_SPARC_LEON3_IRQ_H__

#include "ioports.h"

#define IRQMP_BASE 0x80000200 /**< Leon3 IRQMP IO adress */

#define IRQMP_CLEAR_OFFSET 0x10U /**< Clear register offset */
#define IRQMP_MASK0_OFFSET 0x40U /**< Mask register offset */

/**
 * @def unmask_irq(irq_nbr)
 * Unmask the given irq.
 */
#define unmask_irq(irq_nbr) outw(IRQMP_BASE + IRQMP_MASK0_OFFSET,       \
                                 inb(IRQMP_BASE + IRQMP_MASK0_OFFSET) | (1 << (irq_nbr)))

/**
 * @def ack_irq(irq_nbr)
 * Acknowledge the given irq.
 */
#define ack_irq(irq_nbr) outw(IRQMP_BASE + IRQMP_CLEAR_OFFSET, (1 << (irq_nbr)))

#endif /* __POK_SPARC_LEON3_IRQ_H__ */
