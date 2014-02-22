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
 * @brief  Leon3 timer management.
 */

#include <errno.h>
#include <bsp.h>
#include <core/time.h>
#include <core/sched.h>
#include <arch.h>

#include "ioports.h"
#include "sparc_conf.h"
#include "timer.h"
#include "irq.h"
#include "../traps.h"

/**
 * Timer interrupt subroutine.
 * @see ack_irq(irq_nbr)
 */
void timer_isr(void)
{
  ack_irq(TIMER_IRQ);
  CLOCK_HANDLER
  return;
}

/**
 * Initialize the timer, register the ISR and unmask the interrupt.
 * @see unmask_irq(irq_nbr)
 */
pok_ret_t pok_bsp_time_init ()
{
  outw(TIMER1 + TIMER_SCALER_OFFSET, 1);
  outw(TIMER1 + TIMER_SCAL_RELOAD_OFFSET, 1);

  outw(TIMER1 + TIMER_CNT_VAL_OFFSET, 1);
  outw(TIMER1 + TIMER_RELOAD_OFFSET, SPARC_PROC_FREQ / POK_TIMER_FREQUENCY);
  outw(TIMER1 + TIMER_CTRL_OFFSET,
       TIMER_CTRL_EN | TIMER_CTRL_RS | TIMER_CTRL_LD | TIMER_CTRL_IE);

  pok_arch_event_register(SPARC_TRAP_IRQ_BASE + TIMER_IRQ, timer_isr);
  unmask_irq(TIMER_IRQ);
  return (POK_ERRNO_OK);
}
