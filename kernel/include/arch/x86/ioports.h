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

#ifndef __POK_IOPORTS_H__
#define __POK_IOPORTS_H__

#include <core/syscall.h>

#define outb(port, data) asm volatile("outb %b0,%w1" : : "a"(data), "d"(port))

#define inb(port)                                                              \
  ({                                                                           \
    unsigned char res;                                                         \
    asm volatile("inb %w1,%0" : "=a"(res) : "d"(port));                        \
    res;                                                                       \
  })

#define outl(port, data) asm volatile("outl %0,%w1" : : "a"(data), "d"(port))

#define inl(port)                                                              \
  ({                                                                           \
    unsigned int res;                                                          \
    asm volatile("inl %w1,%0" : "=a"(res) : "d"(port));                        \
    res;                                                                       \
  })

#endif /* __POK_IOPORTS_H__ */
