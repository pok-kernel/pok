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
 * Created by laurent on Sun Jun 07 18:39:49 2009 
 */


#ifdef POK_NEEDS_IO

# include <arch/x86/ioports.h>
# include <libc/stdio.h>

pok_ret_t pok_inb (uint16_t port)
{
  int32_t ret = (int32_t) pok_syscall2 (POK_SYSCALL_INB,
					(uint32_t)port,
					0);

  if (ret < 0)
    printf("pok_inb: error %d, port %x\n", ret, port);

  return (ret);
}

pok_ret_t pok_outb (uint16_t port, uint8_t value)
{
  int32_t ret = (int32_t) pok_syscall2 (POK_SYSCALL_OUTB,
					(uint32_t)port,
					(uint32_t)value);

  if (ret < 0)
    printf("pok_outb: error %d, port %x\n", ret, port);

  return (ret);
}

#endif
