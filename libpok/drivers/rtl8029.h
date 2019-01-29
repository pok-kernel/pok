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

#ifdef POK_NEEDS_RTL8029
# ifndef __POK_RTL8029_H__
#  define __POK_RTL8029_H__

#  include <middleware/port.h>

void rtl8029_init ();
void rtl8029_poll ();
void rtl8029_clear ();

void rtl8029_polling ();
void rtl8029_write (pok_port_id_t port_id, const void* data, uint32_t len);
void rtl8029_read (pok_port_id_t port_id, void* data, uint32_t len);
void rtl8029_poll_and_read (pok_port_id_t port_id, void* data, uint32_t len);

# endif /* !__POK_RTL8029_H__ */
#endif /* POK_NEEDS_RTL8029 */
