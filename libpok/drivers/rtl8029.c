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
 * Created by julien on Mon May 18 18:44:51 2009
 */

/**
 * @file
 * @author Laurent
 * @brief  RTL8029 driver
 * @date   PFE GISTR 2010
 */

#ifdef POK_NEEDS_RTL8029
#include "rtl8029.h"

#include <middleware/port.h>

void rtl8029_poll_and_read (pok_port_id_t port_id, void* data, uint32_t len)
{
  pok_syscall3(POK_SYSCALL_RTL8929_POLL_AND_READ,
	       (uint32_t) port_id,
	       (uint32_t) data,
	       len);
}
/**
 *  @brief Reads data from the corresponding network stack
 *
 *  Reads enqueued data in the stack partition.
 */
void rtl8029_read (pok_port_id_t port_id, void* data, uint32_t len)
{
  pok_syscall3(POK_SYSCALL_RTL8929_READ,
	       (uint32_t) port_id,
	       (uint32_t) data,
	       len);
}

/**
 *  @brief Send data to the interface
 *
 *  Writes data to be sent to network.
 */
void rtl8029_write (pok_port_id_t port_id, const void* data, uint32_t len)
{
  pok_syscall3(POK_SYSCALL_RTL8929_WRITE,
	       (uint32_t) port_id,
	       (uint32_t) data,
	       len);
}

/**
 *  @brief Continuously polls rtl8029 device.
 *
 *  Continuously watches for events, typically for receiving queued packets.
 */
void rtl8029_polling ()
{
  while(1)
  {
    rtl8029_poll();
  }
  rtl8029_clear();
}

/**
 *  @brief Polls rtl8029 device.
 *
 *  Watches for events, typically for receiving queued packets.
 */

void rtl8029_poll ()
{
  pok_do_syscall(POK_SYSCALL_RTL8929_POLL, NULL);
}

/**
 *  @brief Clears rtl8029 device.
 *
 *  Clears rtl8029 registers
 */
void rtl8029_clear ()
{
  pok_do_syscall(POK_SYSCALL_RTL8929_CLEAR, NULL);
}

/**
 *  @brief Initializes rtl8029 device.
 *
 *  Seeks and registers PCI interface, set configuration and fills the
 *  dev structure.
 */
void rtl8029_init ()
{
  pok_do_syscall(POK_SYSCALL_RTL8929_INIT, NULL);
}

#endif
