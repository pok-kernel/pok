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
 * Created by peter on Fri Nov  6 21:20:14 2009
 */

/**
 * @file
 * @author Fabien Chouteau
 * @brief  Traps management.
 */

#include <types.h>
#include <errno.h>
#include <libc.h>
#include <core/debug.h>

#include "thread.h"
#include "traps.h"

/**
 * Interrupt subroutine table.
 */
sparc_traps_handler pok_sparc_isr[256];

/**
 * Initialize ISR table.
 * @see pok_sparc_isr
 */
pok_ret_t traps_init(void)
{
  memset((unsigned char *)pok_sparc_isr, 0x0, sizeof (pok_sparc_isr));
  return POK_ERRNO_OK;
}

/**
 * Function called by interrupt pre-handler.
 * Call the correct handler for the given trap number.
 * @param trap_nb The number of the current trap. (cf SPARC V8 Manual, page 76)
 * @param stack_pointer Adress of the interrupted stack.
 * @see pok_arch_sp
 */
void trap_handler(unsigned int pc,
                  unsigned int npc,
                  unsigned int psr,
                  unsigned int trap_nb,
                  unsigned int restore_counter,
                  unsigned int stack_pointer)
{
  (void)restore_counter;

  pok_arch_sp = stack_pointer;

  if (pok_sparc_isr[trap_nb] != NULL)
  {
    pok_sparc_isr[trap_nb]();
  }
  else
  {
#ifdef POK_NEEDS_DEBUG
    printf ("[KERNEL] [ERROR] Unhandled trap: 0x%x %%PSR=%x %%PC=%x %%nPC=%x %%sp=0x%x\n", trap_nb, psr, pc, npc, stack_pointer);
    printf("%%psr : impl:0x%x ver:%x nzvc:%u%u%u%u EC:%u EF:%u PIL:0x%x S:%u PS:%u ET:%u CWP:%u\n\r",
           (psr >> 28) & 0xF, (psr >> 24) & 0xF,
           (psr >> 23) & 0x1, (psr >> 22) & 0x1c, (psr >> 21) & 0x1, (psr >> 20) & 0x1,
           (psr >> 23) & 0x1, (psr >> 12) & 0x1, (psr >> 8) & 0xF, (psr >> 7) & 0x1, (psr >> 6) & 0x1,
           (psr >> 5) & 0x1, psr & 0xF);
#else
    (void)psr;
    (void)npc;
    (void)pc;
#endif
    POK_FATAL ("Unhandled trap");
  }
  return;
}
