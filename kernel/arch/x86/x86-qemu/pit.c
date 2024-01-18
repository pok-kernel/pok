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
 *                                      Copyright (c) 2007-2024 POK team
 */

#include <arch/x86/interrupt.h>
#include <arch/x86/ioports.h>
#include <assert.h>
#include <bsp.h>
#include <core/sched.h>
#include <core/time.h>
#include <errno.h>

#include "pic.h"

#include "pit.h"

#define OSCILLATOR_RATE 1193182LL // The oscillation rate of x86 clock (Hz)

#define NS_ONE_SECOND 1000000000LL

// following variables are constants, not declared as such for
// retro-compatibility with some versions of GCC; not defined as
// maccro in order to avoid recomputing their values in interrupt
// handler.
static uint32_t OSCILLATOR_DIVISOR;
static uint32_t NS_INCREMENT;
static uint32_t NS_QUANTUM;

#define PIT_BASE 0x40
#define PIT_IRQ 0

INTERRUPT_HANDLER(pit_interrupt) {

  static uint32_t quantum_counter;
  (void)frame;
  pok_pic_eoi(PIT_IRQ);

  pok_tick_counter += NS_INCREMENT;
  quantum_counter += NS_INCREMENT;
  if (quantum_counter >= NS_QUANTUM) {
    quantum_counter -= NS_QUANTUM;
    pok_global_sched();
  }
}

pok_ret_t pok_x86_qemu_timer_init() {
  // OSCILLATOR_DIVISOR, NS_INCREMENT and NS_QUANTUM are constants;
  // but for reasons of compatibility with legacy gcc version, we do
  // the initialization here with static global variables that are not
  // const.
  OSCILLATOR_DIVISOR = OSCILLATOR_RATE / POK_TIMER_FREQUENCY;
  NS_INCREMENT = OSCILLATOR_DIVISOR * NS_ONE_SECOND / OSCILLATOR_RATE;
  NS_QUANTUM = NS_ONE_SECOND / POK_TIMER_QUANTUM;

  // Sanity checks
  assert(OSCILLATOR_DIVISOR <= 65536);
  assert(NS_QUANTUM >= NS_INCREMENT);

  outb(PIT_BASE + 3, 0x34); /* Channel0, rate generator, Set LSB then MSB */
  outb(PIT_BASE, OSCILLATOR_DIVISOR & 0xff);
  outb(PIT_BASE, (OSCILLATOR_DIVISOR >> 8) & 0xff);

  pok_bsp_irq_register(PIT_IRQ, pit_interrupt);

  return POK_ERRNO_OK;
}
