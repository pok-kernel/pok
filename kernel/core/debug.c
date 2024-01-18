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
 *                                      Copyright (c) 2007-2024 POK team
 */

#ifdef POK_NEEDS_DEBUG

#include <arch.h>
#include <core/cons.h>
#include <core/debug.h>
#include <core/multiprocessing.h>
#include <core/partition.h>
#include <core/sched.h>
#include <core/thread.h>
#include <errno.h>

int debug_strlen(const char *str) {
  int i = 0;

  while (*str != '\0') {
    str++;
    i++;
  }
  return i;
}

void pok_debug_print_current_state() {
  uint32_t i;
  uint8_t current_proc = pok_get_proc_id();
  printf("\nCurrent state\n");
  printf("-------------\n");
  printf("Processor            : %d\n", current_proc);
  printf("Kernel thread        : %d\n", KERNEL_THREAD);
  printf("Idle thread          : %d\n", IDLE_THREAD);
  printf("Current partition    : %d\n", POK_SCHED_CURRENT_PARTITION);
  printf("Thread index         : %d\n", POK_CURRENT_PARTITION.thread_index);
  printf("Thread low           : %d\n", POK_CURRENT_PARTITION.thread_index_low);
  printf("Thread high          : %d\n",
         POK_CURRENT_PARTITION.thread_index_high);
  printf("Thread capacity      : %d\n", POK_CURRENT_PARTITION.nthreads);
  printf("Base addr            : 0x%x\n", POK_CURRENT_PARTITION.base_addr);
  printf("Base vaddr           : 0x%x\n", POK_CURRENT_PARTITION.base_vaddr);
  printf("Size                 : %d\n", POK_CURRENT_PARTITION.size);
  printf("Current thread       : %d\n", CURRENT_THREAD(POK_CURRENT_PARTITION));
  printf("Prev current thread  : %d\n", PREV_THREAD(POK_CURRENT_PARTITION));
  printf("Main thread          : %d\n", POK_CURRENT_PARTITION.thread_main);
  printf("Main thread entry    : 0x%x\n",
         POK_CURRENT_PARTITION.thread_main_entry);
  printf("Partition threads sp :");
  for (i = POK_CURRENT_PARTITION.thread_index_low;
       i < POK_CURRENT_PARTITION.thread_index_low +
               POK_CURRENT_PARTITION.thread_index;
       i++) {
    printf(" 0x%x", pok_threads[i].sp);
  }
  printf("\n");
  printf("-------------\n");
  printf("Current thread    : %d\n", POK_SCHED_CURRENT_THREAD);
  printf("Period            : %lld\n", POK_CURRENT_THREAD.period);
  printf("Deadline          : %lld\n", POK_CURRENT_THREAD.deadline);
  printf("Partition         : %d\n", POK_CURRENT_THREAD.partition);
  printf("sp                : 0x%x\n", POK_CURRENT_THREAD.sp);
  printf("init_stack_addr   : 0x%x\n", POK_CURRENT_THREAD.init_stack_addr);
  printf("entry             : 0x%lx\n",
         (unsigned long int)POK_CURRENT_THREAD.entry);
}

void pok_fatal(const char *message) {
  pok_write("FATAL ERROR: \n", 13);
  pok_write(message, debug_strlen(message));

  POK_DEBUG_PRINT_CURRENT_STATE
  pok_arch_idle();
}

#endif /* POK_CONFIG_NEEDS_DEBUG */
