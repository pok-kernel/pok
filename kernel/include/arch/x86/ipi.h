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
 *                                      Copyright (c) 2007-2020 POK team
 */

#ifndef __POK_IPI__

#define POK_IPI_INT_NUMBER 32

pok_ret_t pok_ipi_init(void);
pok_ret_t pok_send_ipi(uint8_t dest_apic, uint32_t lapic_address);
uint8_t check_apic(void);
void enable_apic(void);
#endif /* __POK_IPI__ */