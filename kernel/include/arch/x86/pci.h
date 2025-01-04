/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifdef POK_NEEDS_PCI

#ifndef __POK_X86_PCI_H__
#define __POK_X86_PCI_H__

#include <arch/x86/ioports.h>
#include <core/partition.h>
#include <core/syscall.h>

/*
 * PCI configuration registers
 */
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

/*
 * Configuration space registers
 */
#define PCI_REG_VENDORID 0x00
#define PCI_REG_DEVICEID 0x02
#define PCI_REG_HEADERTYPE 0x0E
#define PCI_REG_BAR0 0x10
#define PCI_REG_IRQLINE 0x3C

/*
 * Useful defines...
 */
#define PCI_BUS_MAX 8
#define PCI_DEV_MAX 32
#define PCI_FUN_MAX 8

/*
 * Structure to holds some device information
 */
typedef struct {
  uint16_t bus;
  uint16_t dev;
  uint16_t fun;
  uint16_t vendorid;
  uint16_t deviceid;
  uint16_t irq_line;
  uint16_t io_range;
  uint32_t bar[6];
  uint32_t addr;
  void *irq_handler;
} s_pci_device;

pok_ret_t pci_register(s_pci_device *dev, uint8_t part_id);

#endif /* __POK_X86_PCI_H__ */
#endif /* POK_NEEDS_PCI */
