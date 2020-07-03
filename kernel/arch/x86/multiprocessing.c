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

/**
 * \file    arch/x86/multiprocessing.c
 * \author  Romain Guilloteau
 * \brief   Check if the system is multiprocessors and configure other
 * processors in this case
 */

#include <arch/x86/multiprocessing.h>
#include <libc.h>

/**
 * \brief This method looks for "_MP_" string inside EBDA memory zone, if this
 * zone exists. This is the signature of the MP floating structure
 * \return The address of the MP floating structure if it has been found, else
 * it returns 0.
 */
int32_t search_mp_ebda() {
  uint32_t ebda_addr;
  // Check if EBDA exists
  ebda_addr = (BDA[0x0F] << 8) | BDA[0x0E];
  if (ebda_addr < 0x80000)
    ebda_addr <<= 4;
  if (ebda_addr == 0)
    return 0;

  // Look for "_MP_" in EBDA
  char *ebda = (char *)ebda_addr;
  for (uint32_t i = 0; i < 1021; i++) {
    if (ebda[i] == '_' && ebda[i + 1] == 'M' && ebda[i + 2] == 'P' &&
        ebda[i + 3] == '_')
      return i;
  }
  return 0;
}

/**
 * \brief This method looks for "_MP_" string inside BIOS ROM memory zone. This
 * is the signature of the MP floating structure \return The address of the MP
 * floating structure if it has been found, else it returns 0.
 */
int32_t search_mp_bios() {
  for (uint32_t i = 0; i < 0xFFFF - 3; i++) {
    if (BIOS[i] == '_' && BIOS[i + 1] == 'M' && BIOS[i + 2] == 'P' &&
        BIOS[i + 3] == '_')
      return (int32_t)BIOS + i;
  }
  return 0;
}

/**
 *\brief   Check if the system is multiprocessors and configure other processors
 *in this case
 */
void pok_multiprocessing_init() {
  int32_t mp;
  printf("\nMultiprocessing init\n");
  if (!(mp = search_mp_ebda()))
    mp = search_mp_bios();
  if (mp) {
    // MP was found, the system is MultiProcessing Compliant
    mp_floating *mp_float = (mp_floating *)mp;
    int proc_enable_number = 0;
    processor_entry *current_proc;
    io_apic_entry *current_io_apic;
    uint32_t current_addr =
        (uint32_t)mp_float->conf_table + sizeof(mp_conf_table_header);
    // Check every entry of the Configuration Table
    for (int i = 0; i < mp_float->conf_table->entry_count; i++) {
      switch (*(uint8_t *)current_addr) {
      case 0:
        current_proc = (processor_entry *)current_addr;
        printf("Entry type: %hhx, Proc n°%d, lapic_id=%hhx, "
               "lapic_version=%hhx, cpu_flags=%hhx, lapic on-chip: %hhx\n",
               current_proc->entry_type, i, current_proc->lapic_id,
               current_proc->lapic_version, current_proc->cpu_flags,
               READ_BIT(current_proc->feature_flags, 9));

        if (READ_BIT(current_proc->cpu_flags, 0))
          proc_enable_number++;

        current_addr += 20;
        break;
      case 2:
        current_io_apic = (io_apic_entry *)current_addr;
        if (current_io_apic->enable) {
          printf("IO APIC at %x\n", current_io_apic->address);
        }
        current_addr += 8;
        break;
      default:
        current_addr += 8;
        break;
      }
    }
    printf("LAPIC at %x\n", mp_float->conf_table->lapic_addr);

    if (proc_enable_number == 1)
      return;

    if (READ_BIT(
            *(volatile uint32_t *)(mp_float->conf_table->lapic_addr + 0xF0), 8))
      printf("LAPIC already enable\n");
  }
}
