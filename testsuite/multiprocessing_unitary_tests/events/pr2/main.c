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

#include <assert.h>
#include <core/event.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/time.h>
#include <libc/stdio.h>
#include <types.h>

#ifndef AFFINITY_0
#define AFFINITY_0 0
#endif

#ifndef AFFINITY_1
#define AFFINITY_1 1
#endif

static pok_event_id_t eid;
uint8_t signal, receive, fail = 0;

static void *pinger_job() {
  printf("Hello from P2T1!\n");
  pok_thread_sleep(5000);
  for (signal = 1; signal <= 5; signal++) {
    printf("P2T1: I signal event (signal %d)\n", signal);
    pok_event_signal(eid);
    pok_thread_sleep(30000);
    assert(signal == receive);
    assert(signal == fail);
  }
  printf("P2T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  pok_ret_t ret;
  printf("Hello from P2T2!\n");
  while (fail < 5) {
    pok_event_lock(eid);
    printf("P2T2: I will wait for the signal\n");
    ret = pok_event_wait(eid, 20000);
    if (!ret) {
      printf("P2T2: got the event. Success %d\n", ++receive);
    } else {
      printf("P2T2: didn't get the event. Fail %d\n", ++fail);
    }
    pok_event_unlock(eid);
  }
  printf("P2T2: sleeping forever\n");
  if (signal == 5 && receive == 5 && fail == 5)
    printf("SUCCESS!!!\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;

  ret = pok_event_create(&eid);
  printf("[P2] pok_event_create return=%d, mid=%d\n", ret, eid);

  tattr.priority = 40;
  tattr.entry = pinger_job;
  tattr.processor_affinity = AFFINITY_0;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;
  tattr.processor_affinity = AFFINITY_1;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (2) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
