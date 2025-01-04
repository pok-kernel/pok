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

#ifndef __POK_IOPORTS_H__
#define __POK_IOPORTS_H__

#define POK_PREP_IOBASE 0x80000000

#define outb(port, data)                                                       \
  *((volatile unsigned char *)(POK_PREP_IOBASE + port)) = data

#define inb(port) *((volatile unsigned char *)(POK_PREP_IOBASE + port))

#endif /* __POK_IOPORTS_H__ */
