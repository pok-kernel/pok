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


#include <libc.h>

#include <types.h>
#include <errno.h>
#include <core/syscall.h>

#include "event.h"
#include "sysdesc.h"

#define IDT_SIZE 256

idt_entry_t	pok_idt[IDT_SIZE];

pok_ret_t pok_event_init ()
{
   pok_idt_init ();

#if defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_ERROR_HANDLING)
   pok_exception_init ();
#endif

   pok_syscall_init ();

   return (POK_ERRNO_OK);
}

pok_ret_t pok_idt_init ()
{
   sysdesc_t sysdesc;

   /* Clear table */
   memset(pok_idt, 0, sizeof (idt_entry_t) * IDT_SIZE);

   /* Load IDT */
   sysdesc.limit = sizeof (pok_idt);
   sysdesc.base = (uint32_t)pok_idt;

   asm ("lidt %0"
        :
        : "m" (sysdesc));

  return (POK_ERRNO_OK);
}

void pok_idt_set_gate (uint16_t     index,
                       uint16_t     segsel,
                       uint32_t     offset,
                       e_idte_type  t,
                       int          dpl)
{
   pok_idt[index].offset_low   = (offset) & 0xFFFF;
   pok_idt[index].offset_high  = (offset >> 16) & 0xFFFF;
   pok_idt[index].segsel       = segsel;
   pok_idt[index].dpl          = dpl;
   pok_idt[index].type         = t;
   pok_idt[index].d            = 1;
   pok_idt[index].res0         = 0; /* reserved */
   pok_idt[index].res1         = 0; /* reserved */
   pok_idt[index].present   = 1;
}

