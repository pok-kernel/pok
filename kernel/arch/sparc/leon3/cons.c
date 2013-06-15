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

/**
 * @file
 * @author Fabien Chouteau
 * @brief  Leon3 UART driver.
 */

#include <errno.h>

#include "ioports.h"
#include <libc.h>
#include <core/debug.h>
#include <core/cons.h>
#include "cons.h"

#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

static void write_serial(char a)
{
  while ((inw(UART1 + UART_STAT_OFFSET) & UART_STATUS_THE) == 0)
    continue;

  outb(UART1 + UART_DATA_OFFSET, (uint8_t)a);
}

pok_bool_t pok_cons_write (const char *s, size_t length)
{
  for (; length > 0; length--)
    write_serial (*s++);
  return 0;
}


/**
 * UART initialization.
 * Keep default baud rate value (38400).
 */
int pok_cons_init (void)
{
  outw(UART1 + UART_CTRL_OFFSET, UART_CTRL_TE); /* transmit enable */
  outw(UART1 + UART_STAT_OFFSET, 0);

  pok_print_init (write_serial, NULL);
  return 0;
}
#else
int pok_cons_init (void)
{
  return 0;
}
#endif


