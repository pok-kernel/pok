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

#include "event.h"
#include "gdt.h"
#include <arch/x86/ipi.h>
#include <arch/x86/multiprocessing.h>
#include <errno.h>
#include <libc.h>
#include <types.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_ENABLE 0x800
#define MSR_FEAT_APIC (1 << 11)

INTERRUPT_HANDLER(IPI_gate) {
  (void)frame;
  printf("Coucou!!\n");
}

pok_ret_t pok_ipi_init() {
  pok_idt_set_gate(POK_IPI_INT_NUMBER, GDT_CORE_CODE_SEGMENT << 3,
                   (uint32_t)IPI_gate, IDTE_INTERRUPT, 3);
  return (POK_ERRNO_OK);
}

pok_ret_t pok_send_ipi(uint8_t dest_apic, uint32_t lapic_address) {
  *(uint32_t *)(lapic_address + 0x310) = (dest_apic << (32 - 8));
  *(uint32_t *)(lapic_address + 0x300) =
      0b00000100000000000000 + POK_IPI_INT_NUMBER;
  return (POK_ERRNO_OK);
}

/**
 * @brief Got from https://wiki.osdev.org/Model_Specific_Registers
 */
void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
  asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

/**
 * @brief Got from https://wiki.osdev.org/Model_Specific_Registers
 */
void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
  asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

/**
 * @brief Check if LAPIC is hardware enable (from MSR) and software enable (from
 * Spurious Vector 0xFEE000F0). Inspired from https://wiki.osdev.org/APIC
 */
uint8_t check_apic() {
  uint32_t eax, edx;
  cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
  return ((eax & MSR_FEAT_APIC) >> 11) &
         READ_BIT(*(volatile uint32_t *)(0xfee00000 + 0xF0), 8);
}

/**
 * @brief Hardware enable the LAPIC
 */
void cpu_hardware_enable_apic() {
  uint32_t eax, edx;
  cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

  edx = 0;
  eax = eax | IA32_APIC_BASE_MSR_ENABLE;
  cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

/**
 * @brief Hardware and software enable the LAPIC
 */
void enable_apic() {
  /* Hardware enable the Local APIC if it wasn't enabled */
  cpu_hardware_enable_apic();
  /* Set the Spurious Interrupt Vector Register bit 8 to start receiving
   * interrupts */
  SET_BIT(*(volatile uint32_t *)(0xfee00000 + 0xF0), 8);
}