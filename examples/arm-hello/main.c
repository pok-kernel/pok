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

/**
 * \file examples/arm-hello/main.c
 * \brief Simple ARM Cortex-M hello world example
 */

#include <core/partition.h>
#include <core/thread.h>
#include <core/time.h>
#include <libc/stdio.h>
#include <string.h>

void thread1_job(void) {
  int i = 0;

  while (1) {
    printf("Hello from ARM Cortex-M thread 1, iteration %d\n", i++);
    pok_thread_sleep(1000000); /* Sleep for 1 second */
  }
}

void thread2_job(void) {
  int i = 0;

  while (1) {
    printf("Hello from ARM Cortex-M thread 2, iteration %d\n", i++);
    pok_thread_sleep(1500000); /* Sleep for 1.5 seconds */
  }
}

static inline void setup_thread_attributes(pok_thread_attr_t *attr,
                                           void (*func_ptr)(void)) {
  /* Non-portable hack: copy function pointer bytes to void* since
   * pok_thread_attr_t.entry is void* but we need to pass function pointers */
  memcpy(&attr->entry, &func_ptr, sizeof attr->entry);
  attr->priority = 1;
  attr->stack_size = 2048;
  attr->processor_affinity = 0;
  attr->period = 0;
  attr->deadline = 0;
  attr->time_capacity = 0;
  attr->state = POK_STATE_RUNNABLE;
}

int main(void) {
  pok_ret_t ret;
  uint32_t tid1, tid2;
  pok_thread_attr_t attr1 = {0}, attr2 = {0};

  printf("POK ARM Cortex-M Hello World Example\n");
  printf("====================================\n");

  /* Setup first thread attributes */
  setup_thread_attributes(&attr1, thread1_job);

  /* Create first thread */
  ret = pok_thread_create(&tid1, &attr1);
  if (ret != POK_ERRNO_OK) {
    printf("Error creating thread 1: %d\n", ret);
    return -1;
  }

  /* Setup second thread attributes */
  setup_thread_attributes(&attr2, thread2_job);

  /* Create second thread */
  ret = pok_thread_create(&tid2, &attr2);
  if (ret != POK_ERRNO_OK) {
    printf("Error creating thread 2: %d\n", ret);
    return -1;
  }

  printf("Threads created successfully. Starting scheduler...\n");

  /* Start the threads. This call does not return. */
  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  /* Should never reach here - partition mode change transfers control to
   * scheduler */
  __builtin_unreachable();
}
