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

#include <assert.h>
#include <core/partition.h>
#include <core/semaphore.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <types.h>

#ifndef AFFINITY_0
#define AFFINITY_0 0
#endif

#ifndef AFFINITY_1
#define AFFINITY_1 0
#endif

static uint8_t sid;
static int i;
static void *pinger_job() {
  for (i = 1; i <= 5; i++) {
    pok_thread_sleep(10000);
    printf("P1T1: I will signal semaphore (round %d)\n", i);
    pok_sem_signal(sid);
    printf("P1T1: semaphore signaled (round %d)\n", i);
  }
  printf("P1T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  for (int j = 1; j <= 5; j++) {
    printf("P1T2: I will wait for the semaphore (round %d)\n", j);
    pok_sem_wait(sid, 0);
    printf("P1T2: got the semaphore (round %d)\n", j);
    assert(i == j);
  }
  printf("P1T2: sleeping forever\n");
  printf("SUCCESS!!!");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;

  ret = pok_sem_create(&sid, 0, 50, POK_QUEUEING_DISCIPLINE_DEFAULT);
  printf("[P1] pok_sem_create return=%d, mid=%d\n", ret, sid);

  tattr.priority = 40;
  tattr.entry = pinger_job;
  tattr.processor_affinity = AFFINITY_0;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;
  tattr.processor_affinity = AFFINITY_1;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (2) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
