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


#include <types.h>
#include <errno.h>

#include <arch/x86/ioports.h>

#include "pic.h"

int pok_pic_init ()
{
   outb (PIC_MASTER_BASE, PIC_MASTER_ICW1);
   outb (PIC_SLAVE_BASE, PIC_SLAVE_ICW1);

   outb (PIC_MASTER_BASE + 1, PIC_MASTER_ICW2);
   outb (PIC_SLAVE_BASE + 1, PIC_SLAVE_ICW2);

   outb (PIC_MASTER_BASE + 1, PIC_MASTER_ICW3);
   outb (PIC_SLAVE_BASE + 1, PIC_SLAVE_ICW3);

   outb (PIC_MASTER_BASE + 1, PIC_MASTER_ICW4);
   outb (PIC_SLAVE_BASE + 1, PIC_SLAVE_ICW4);

   /* Mask everything */
   outb (PIC_MASTER_BASE + 1, 0xfb);
   outb (PIC_SLAVE_BASE + 1, 0xff);

   return (POK_ERRNO_OK);
}

int pok_pic_mask (uint8_t irq)
{
   uint8_t mask;

   if (irq > 15)
   {
      return (POK_ERRNO_EINVAL);
   }

   if (irq < 8)
   {
      mask = inb (PIC_MASTER_BASE + 1);
      outb (PIC_MASTER_BASE + 1, mask | (1 << irq));
   }
   else
   {
      mask = inb (PIC_SLAVE_BASE + 1);
      outb (PIC_SLAVE_BASE + 1, mask | (1 << (irq - 8)));
   }

   return (POK_ERRNO_OK);
}

int pok_pic_unmask(uint8_t irq)
{
   uint8_t mask;

   if (irq > 15)
      return (POK_ERRNO_EINVAL);

   if (irq < 8)
   {
      mask = inb(PIC_MASTER_BASE + 1);
      outb(PIC_MASTER_BASE + 1, mask & ~(1 << irq));
   }
   else
   {
      mask = inb(PIC_SLAVE_BASE + 1);
      outb(PIC_SLAVE_BASE + 1, mask & ~(1 << (irq - 8)));
   }

   return (POK_ERRNO_OK);
}

void pok_pic_eoi (uint8_t irq)
{
   if (irq >= 8)
   {
      outb (PIC_SLAVE_BASE, 0x20);
   }

   outb (PIC_MASTER_BASE, 0x20);
}

