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
 *                                      Copyright (c) 2007-2022 POK team
 */

#ifndef __POK_IPI__

#define POK_IPI_TEST_INT_NUMBER 33
#define POK_IPI_GLOBAL_SCHED_INT_NUMBER 34
#define POK_IPI_SCHED_THREAD_INT_NUMBER 35

#define IPI_NOGROUP 0
#define IPI_SELF 1
#define IPI_ALL 2
#define IPI_ALL_WITHOUT_SELF 3

pok_ret_t pok_ipi_init(void);
pok_ret_t pok_send_ipi(uint8_t dest_group, uint8_t dest_apic,
                       uint8_t INT_number);
uint8_t check_apic(void);
void enable_apic(void);
void pok_end_ipi(void);

#define INTERRUPT_HANDLER_IPI(name)                                            \
  void name(void);                                                             \
  void name##_handler(interrupt_frame *frame);                                 \
  asm(".global " #name "			\n"                                                 \
      "\t.type " #name ",@function	\n" #name                              \
      ":				\n"                                                                \
      "cli			\n"                                                               \
      "subl $4, %esp			\n"                                                     \
      "pusha				\n"                                                            \
      "push %ds				\n"                                                         \
      "push %es				\n"                                                         \
      "push %esp			\n"                                                         \
      "mov $0x10, %ax			\n"                                                    \
      "mov %ax, %ds			\n"                                                      \
      "mov %ax, %es			\n"                                                      \
      "call " #name "_handler		\n"                                             \
      "call update_tss			\n"                                                   \
      "addl $4, %esp			\n"                                                     \
      "pop %es				\n"                                                          \
      "pop %ds				\n"                                                          \
      "popa				\n"                                                             \
      "addl $4, %esp			\n"                                                     \
      "sti			\n"                                                               \
      "call pok_end_ipi  \n"                                                   \
      "iret				\n");                                  \
  void name##_handler(interrupt_frame *frame)

#endif /* __POK_IPI__ */
