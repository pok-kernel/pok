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

#include <core/event.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/time.h>
#include <libc/stdio.h>
#include <types.h>

static pok_event_id_t eid;

static void *pinger_job() {
  printf("Hello from P2T1!\n");
  for (int j = 1; j <= 5; j++) {
    printf("P2T1: I signal event (signal %d)\n", j);
    pok_event_signal(eid);
    pok_thread_sleep(15000);
  }
  printf("P2T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  pok_ret_t ret;
  int fail = 0;
  printf("Hello from P2T2!\n");
  pok_event_unlock(eid);
  while (fail < 5) {
    pok_event_lock(eid);
    printf("P2T2: I will wait for the signal\n");
    ret = pok_event_wait(eid, 10000);
    if (!ret) {
      printf("P2T2: got the event\n");
    } else
      printf("P2T2: didn't get the event. Fail %d\n", ++fail);
    pok_event_unlock(eid);
  }
  printf("P2T2: sleeping forever\n");
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

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (2) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
