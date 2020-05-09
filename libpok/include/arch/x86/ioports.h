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

#ifdef POK_NEEDS_IO

#ifndef __POK_X86_IO_H__
#define __POK_X86_IO_H__

#include <core/syscall.h>
#include <types.h>

pok_ret_t pok_inb(uint16_t port);
pok_ret_t pok_outb(uint16_t port, uint8_t value);

#endif /* __POK_X86_IO_H__ */

#endif /* POK_NEEDS_IO */
