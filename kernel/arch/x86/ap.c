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
 *                                      Copyright (c) 2007-2020 POK team
 */

#include "event.h"
#include <arch.h>
#include <arch/x86/ipi.h>
#include <arch/x86/spinlock.h>
#include <assert.h>
#include <core/multiprocessing.h>
#include <libc.h>

extern uint8_t incr_var;
extern uint8_t start_spinlock;
extern uint8_t proc_index[POK_CONFIG_NB_MAX_PROCESSORS];
extern int spinlocks[POK_CONFIG_NB_MAX_PROCESSORS];

/**
 * \brief Main method for APs
 * TODO: implements next steps (Here tests are implemented)
 */
void main_ap(void) {
  SPIN_LOCK(start_spinlock);
  proc_index[pok_get_lapic_id()] = incr_var;
  incr_var++;
  spinlocks[0]--;
  spinlocks[pok_get_proc_id()]++;
  SPIN_UNLOCK(start_spinlock);

  pok_gdt_init(pok_get_proc_id());

  pok_event_init();
  pok_arch_preempt_enable();
  enable_apic();
}
