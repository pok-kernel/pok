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
 * Created by laurent on Mon Jun 08 11:01:12 2009 
 */

#ifdef POK_NEEDS_PCI

# include <libc.h>
# include <arch/x86/pci.h>

# include "gdt.h"
# include "event.h"

static inline
unsigned int pci_read(unsigned int bus,
		      unsigned int dev,
		      unsigned int fun,
		      unsigned int reg)
{
  unsigned int addr = (1 << 31) | (bus << 16) | (dev << 11) | (fun << 8) | (reg & 0xfc);
  unsigned int val = -1;

  outl(PCI_CONFIG_ADDRESS, addr);
  val = inl(PCI_CONFIG_DATA);

  return (val >> ((reg & 3) << 3));
}

static inline
unsigned int pci_read_reg(s_pci_device* d,
			  unsigned int reg)
{
  return (pci_read(d->bus, d->dev, d->fun, reg));
}

static inline
int pci_open(s_pci_device* d)
{
  unsigned int bus = 0;
  unsigned int dev = 0;
  unsigned int fun = 0;

  for (bus = 0; bus < PCI_BUS_MAX; bus++)
    for (dev = 0; dev < PCI_DEV_MAX; dev++)
      for (fun = 0; fun < PCI_FUN_MAX; fun++)
	if (((unsigned short) pci_read(bus, dev, fun, PCI_REG_VENDORID)) == d->vendorid &&
	    ((unsigned short) pci_read(bus, dev, fun, PCI_REG_DEVICEID)) == d->deviceid)
	{
	  // we do not handle type 1 or 2 PCI configuration spaces
	  if (pci_read(bus, dev, fun, PCI_REG_HEADERTYPE) != 0)
	    continue;

	  d->bus = bus;
	  d->dev = dev;
	  d->fun = fun;
	  d->bar[0] = pci_read(bus, dev, fun, PCI_REG_BAR0);
	  d->irq_line = (unsigned char) pci_read_reg(d, PCI_REG_IRQLINE);

	  return (0);
	}

  return (-1);
}

void pci_handler(void);

void dummy_pci_handler(void)
{
  __asm__ volatile
    (
     ".globl pci_handler\n"
     "pci_handler:\n"
     "push %eax\n"		// save restricted context
     "push %edx\n"
     "mov $0x20, %al\n"
     "mov $0xA0, %dx\n"		// ack slave pic
     "outb %al, %dx\n"
     "mov $0x20, %dx\n"		// ack master pic
     "outb %al, %dx\n"
     "pop %edx\n"		// restore retricted context
     "pop %eax\n"
     "iret\n"			// return
    );
}

void* ne2000_dev = NULL;

pok_ret_t pci_register(s_pci_device*	dev,
		       uint8_t		part_id)
{
  if (pci_open(dev) != 0)
    return (-1);

  pok_partitions[part_id].io_min = (dev->bar[0] & (~0x1F));
  pok_partitions[part_id].io_max = (dev->bar[0] & (~0x1F)) + dev->io_range;

  pok_idt_set_gate(32 + dev->irq_line,
		   GDT_CORE_CODE_SEGMENT,
		   (uint32_t) pci_handler,
		   IDTE_INTERRUPT,
		   0);

  ne2000_dev = dev;

  return (0);
}

#endif /* POK_NEEDS_PCI */
