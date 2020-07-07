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
#include <arch/x86/multiprocessing.h>
#include <libc.h>

extern uint8_t *incr_var;

/**
 * \brief Main method for APs
 * TODO: implements next steps (Here tests are implemented)
 */
void main_ap(void) {
  asm("lock incw %0" : "=m"(*incr_var) : "m"(*incr_var));

  pok_event_init();
  pok_arch_preempt_enable();
  enable_apic();
}