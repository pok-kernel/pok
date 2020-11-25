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

#include <arch/x86/ioports.h>
#include <arch/x86/ipi.h>
#include <arch/x86/spinlock.h>
#include <assert.h>
#include <core/multiprocessing.h>
#include <core/partition.h>
#include <core/time.h>
#include <libc.h>

extern void *__realmode_lma_start;
extern void *__realmode_lma_end;
extern void *__REAL_BASE;

uint32_t lapic_address;
uint8_t incr_var;
uint8_t proc_index[POK_CONFIG_NB_PROCESSORS];
int spinlocks[POK_CONFIG_NB_PROCESSORS] = {0};
uint8_t start_spinlock;
uint8_t multiprocessing_system = 0;

#define PIT_BASE 0x40

uint8_t pok_get_lapic_id() {
  return (uint8_t)(*(volatile uint32_t *)(lapic_address + 0x20) >> 24);
};

uint8_t pok_get_proc_id() {
  return multiprocessing_system ? proc_index[pok_get_lapic_id()] : 0;
}

void pok_send_global_schedule_thread() {
  pok_send_ipi(IPI_ALL_WITHOUT_SELF, 0, POK_IPI_GLOBAL_SCHED_INT_NUMBER);
}

void pok_send_schedule_thread(uint8_t dest) {
  pok_send_ipi(IPI_NOGROUP, dest, POK_IPI_SCHED_THREAD_INT_NUMBER);
}

void pok_send_schedule_thread_other_processors() {
  pok_send_ipi(IPI_ALL_WITHOUT_SELF, 0, POK_IPI_SCHED_THREAD_INT_NUMBER);
}

/**
 * \brief This method uses the PIT to realize a wait. It is used to wait between
 * two IPI transmissions
 */
void pok_x86_wait_mp(uint16_t counter_init) {
  outb(PIT_BASE + 3, 0x30);
  outb(PIT_BASE, (uint8_t)counter_init);
  outb(PIT_BASE, (uint8_t)(counter_init >> 8));
  outb(PIT_BASE + 3, 0xE2);
  while (!(inb(PIT_BASE) >> 7)) {
    outb(PIT_BASE + 3, 0xE2);
  }
}

/**
 * \brief This method starts the APs
 */
void pok_start_ap() {
  // Send INIT IPI
  *(uint32_t *)(lapic_address + 0x300) = 0b11001100010111111111;
  *(uint32_t *)(lapic_address + 0x300) = 0b11001000010111111111;
  // Wait 10 milliseconds (OSDEV delay)
  pok_x86_wait_mp(0x2e9b);
  // Send first STARTUP IPI
  *(uint32_t *)(lapic_address + 0x300) = 0x000C4600 + (0x01);
  // Wait 1 millisecond (OSDEV delay)
  pok_x86_wait_mp(0x4AE);
  // Send second STARTUP IPI
  *(uint32_t *)(lapic_address + 0x300) = 0x000C4600 + (0x01);
  // Wait 1 second (OSDEV delay)
  for (char i = 0; i < 19; i++)
    pok_x86_wait_mp(0xffff);
}

void setup_test(void) { incr_var = 1; }

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

void bus_entry_handler(uint32_t current_addr) {
#if POK_NEEDS_DEBUG
  bus_entry *current_bus = (bus_entry *)current_addr;
  if(current_bus != NULL) {

    printf("Bus entry:\nBus ID: %hhx\nBus Type: %c%c%c%c%c%c\n\n",
	   current_bus->id, current_bus->type_string[0],
	   current_bus->type_string[1], current_bus->type_string[2],
	   current_bus->type_string[3], current_bus->type_string[4],
	   current_bus->type_string[5]);
  }
#else
  (void) current_addr; // workaround unused parameter; TODO: remove
		       // when used for other purposes than debug
#endif
}

void io_apic_entry_handler(uint32_t current_addr) {
  io_apic_entry *current_io_apic = (io_apic_entry *)current_addr;
  if (current_io_apic->enable) {
#if POK_NEEDS_DEBUG
    printf("IO APIC at %x\n", current_io_apic->address);
#endif
    uint32_t offset = 0x14;
    *(volatile uint32_t *)current_addr = offset;
#if POK_NEEDS_DEBUG
    printf("LAPIC: %x", *(volatile uint32_t *)(current_addr + offset++));
#endif
    *(volatile uint32_t *)current_addr = offset;
#if POK_NEEDS_DEBUG
    printf(" %x\n", *(volatile uint32_t *)(current_addr + offset));
#endif
  }
}

void io_apic_interrupt_entry_handler(uint32_t current_addr) {
#if POK_NEEDS_DEBUG
  apic_interrupt_entry *current_apic_interrupt =
      (apic_interrupt_entry *)current_addr;
  if(current_api_interrupt != NULL) {

    printf("IO APIC interrupt:\nType: %hhx\nFlags: %hx\nBus ID: %hhx\nBus "
	   "IRQ: "
	   "%hhx\nAPIC: %hhx\nIDINTin: %hhx\n\n",
	   current_apic_interrupt->interrupt_type, current_apic_interrupt->flags,
	   current_apic_interrupt->bus_id, current_apic_interrupt->bus_irq,
	   current_apic_interrupt->apic_id, current_apic_interrupt->apic_intin);
  }
#else
  (void) current_addr; // workaround unused parameter; TODO: remove
		       // when used for other purposes than debug
#endif
}

void lapic_interrupt_entry_handler(uint32_t current_addr) {
#if POK_NEEDS_DEBUG
  apic_interrupt_entry *current_apic_interrupt =
      (apic_interrupt_entry *)current_addr;
  printf("LAPIC interrupt:\nType: %hhx\nFlags: %hx\nBus ID: %hhx\nBus IRQ: "
         "%hhx\nAPIC: %hhx\nIDINTin: %hhx\n\n",
         current_apic_interrupt->interrupt_type, current_apic_interrupt->flags,
         current_apic_interrupt->bus_id, current_apic_interrupt->bus_irq,
         current_apic_interrupt->apic_id, current_apic_interrupt->apic_intin);
#else
  (void) current_addr; // workaround unused parameter; TODO: remove
		       // when used for other purposes than debug
#endif
}

/**
 * \brief Copy realmode data and code from LMA (0x100000) to the VMA (0x1000)
 */
static void realmode_setup(void) {
  /* Each of these __realmode* values is generated by the linker script */
  const uint32_t *src_addr = (uint32_t *)&__realmode_lma_start;
  const uint32_t *src_end = (uint32_t *)&__realmode_lma_end;
  uint32_t *dst_addr = (uint32_t *)&__REAL_BASE;

  while (src_addr < src_end)
    *dst_addr++ = *src_addr++;
}

/**
 *\brief   Check if the system is multiprocessors and configure other processors
 *in this case
 */
void pok_multiprocessing_init() {
  int32_t mp;
#if POK_NEEDS_DEBUG
  printf("\nMultiprocessing init\n");
#endif
  if (!(mp = search_mp_ebda()))
    mp = search_mp_bios();
  if (mp) {
    // MP was found, the system is MultiProcessing Compliant
    mp_floating *mp_float = (mp_floating *)mp;
    int proc_enable_number = 0;
    int proc_number = 0;
    uint32_t current_addr =
        (uint32_t)mp_float->conf_table + sizeof(mp_conf_table_header);
    // Check every entry of the Configuration Table
    for (int i = 0; i < mp_float->conf_table->entry_count; i++) {
      switch (*(uint8_t *)current_addr) {
      case 0: {
        processor_entry *current_proc = (processor_entry *)current_addr;
        if (READ_BIT(current_proc->cpu_flags, 0))
          proc_enable_number++;
        proc_number++;

        current_addr += 20;
        break;
      }

      case 1:
        bus_entry_handler(current_addr);
        current_addr += 8;
        break;

      case 2:
        io_apic_entry_handler(current_addr);
        current_addr += 8;
        break;

      case 3:
        io_apic_interrupt_entry_handler(current_addr);
        current_addr += 8;
        break;

      case 4:
        lapic_interrupt_entry_handler(current_addr);
        current_addr += 8;
        break;

      default:
#if POK_NEEDS_DEBUG
        printf("Error");
#endif
        assert(0);
        break;
      }
    }

    assert(proc_number <= POK_CONFIG_NB_PROCESSORS);
    lapic_address = mp_float->conf_table->lapic_addr;

    multiprocessing_system = proc_enable_number;
    assert(multiprocessing_system == POK_CONFIG_NB_PROCESSORS);

    if (!check_apic())
      enable_apic();

    realmode_setup();
    setup_test();
    pok_ipi_init();

    proc_index[pok_get_lapic_id()] = 0;
    start_spinlock = 0;

    for (int i = 0; i < POK_CONFIG_NB_PROCESSORS; i++)
      spinlocks[i] = 0;

    pok_start_ap();

    // pok_send_ipi(IPI_NOGROUP, 1, POK_IPI_TEST_INT_NUMBER);
    pok_x86_wait_mp(0xffff);

    // Check if each core have incremented incr_var
    assert(incr_var == proc_enable_number);
  }
}
