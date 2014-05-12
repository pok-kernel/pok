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


#ifdef POK_NEEDS_DEBUG

#include <libc.h>
#include "../event.h"

void set_debug_traps (); /* provided by the gdb stub*/

void putDebugChar ()
{
}

int getDebugChar ()
{
   return 0;
}

void exceptionHandler (int exception_number,
                       void* exception_address)
{
   pok_idt_set_gate (exception_number,
                     GDT_CORE_CODE_SEGMENT << 3,
                     (uint32_t)exception_address,
                     IDTE_INTERRUPT,
                     3);
}

void set_debug_serial_port ()
{
}

void pok_bsp_debug ()
{
   set_debug_serial_port ();
   set_debug_traps ();
   printf ("Waiting GDB connection\n");
   __asm__ ("int3");
}

#endif
