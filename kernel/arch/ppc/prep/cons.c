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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#include <errno.h>

#include "ioports.h"
#include <libc.h>
#include <core/debug.h>
#include <core/cons.h>
#include "cons.h"

#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

#define  COM1      0x3F8

static int is_transmit_empty(void)
{
   return inb(COM1 + 5) & 0x20;
}

static void write_serial(char a)
{
   while (is_transmit_empty() == 0)
     ;

   outb(COM1,a);
}

pok_bool_t pok_cons_write (const char *s, size_t length)
{
   for (; length > 0; length--)
      write_serial (*s++);
   return 0;
}


int pok_cons_init (void)
{
   outb(COM1 + 1, 0x00);    // Disable all interrupts
   outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(COM1 + 1, 0x00);    //                  (hi byte)
   outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set

   pok_print_init (write_serial, NULL);

   return 0;


}
#else
int pok_cons_init (void)
{
   return 0;
}
#endif


