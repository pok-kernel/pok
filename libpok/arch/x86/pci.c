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
 * Created by laurent on Mon Jun 08 11:18:27 2009 
 */

#ifdef POK_NEEDS_PCI

# include <arch/x86/pci.h>

pok_ret_t pci_register(s_pci_device* dev)
{
  return pok_syscall2(POK_SYSCALL_PCI_REGISTER,
		     (uint32_t) dev,
		     NULL);
}

#endif /* POK_NEEDS_PCI */
