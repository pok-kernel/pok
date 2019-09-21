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


#include <errno.h>
#include <bsp.h>
#include <core/time.h>
#include <core/sched.h>
#include <arch/x86/ioports.h>
#include <arch/x86/interrupt.h>

#include "pic.h"

#include "pit.h"

#define OSCILLATOR_RATE 1193182 /** The oscillation rate of x86 clock (Hz) */
#define PIT_BASE 0x40
#define PIT_IRQ 0

static uint64_t pok_prev_tick_value, pok_counter_ns_incr, pok_quantum_incr;

uint32_t pit_freq;

INTERRUPT_HANDLER (pit_interrupt)
{
   (void) frame;
   pok_pic_eoi (PIT_IRQ);

   pok_tick_counter += pok_counter_ns_incr;
   if(pok_tick_counter-pok_prev_tick_value>=pok_quantum_incr)
   {
     pok_prev_tick_value = pok_tick_counter;
     pok_sched();
   }
}

pok_ret_t pok_x86_qemu_timer_init ()
{
   pok_counter_ns_incr = 9219; // 9219 because it is 11 (integer division OSCILLATOR_RATE / POK_TIMER_FREQUENCY) divided (floating point) by 1193182 multiplied by 19e9
   pok_quantum_incr = POK_TIMER_QUANTUM*pok_counter_ns_incr;
   outb (PIT_BASE + 3, 0x34); /* Channel0, rate generator, Set LSB then MSB */
   outb (PIT_BASE, (OSCILLATOR_RATE / POK_TIMER_FREQUENCY) & 0xff);
   outb (PIT_BASE, ((OSCILLATOR_RATE / POK_TIMER_FREQUENCY) >> 8) & 0xff);

   pok_bsp_irq_register (PIT_IRQ, pit_interrupt);

   return (POK_ERRNO_OK);
}


