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
 * Created by laurent on Sun Jun 07 18:44:37 2009 
 */

#ifdef POK_NEEDS_IO

# ifndef __POK_x86_IO_H__
#  define __POK_X86_IO_H__

#  include <types.h>
#  include <core/syscall.h>

pok_ret_t pok_inb (uint16_t port);
pok_ret_t pok_outb (uint16_t port, uint8_t value);

# endif /* __POK_X86_IO_H__ */

#endif /* POK_NEEDS_IO */
