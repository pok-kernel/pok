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
 *                                      Copyright (c) 2007-2022 POK team
 */

/**
 * @file
 * @author Fabien Chouteau
 */

#ifndef __POK_SPARC_TRAPS_H__
#define __POK_SPARC_TRAPS_H__

#include <errno.h>
#include <types.h>

#define SPARC_TRAP_IRQ_BASE 0x10
#define SPARC_TRAP_SYSCALL_BASE 0x80

typedef void (*sparc_traps_handler)(void);

extern sparc_traps_handler pok_sparc_isr[256];

pok_ret_t traps_init(void);

#endif /* !__POK_PPC_TRAPS_H__ */
