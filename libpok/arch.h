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

#ifndef __LIBPOK_ARCH__
#define __LIBPOK_ARCH__

void pok_arch_idle(void);
/*
 * Dummy function that halts the CPU
 */

void pok_context_switch(uint32_t *old_sp, uint32_t new_sp);
/*
 * Handle context-switch between two threads
 */

#endif
