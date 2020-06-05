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
#include <libc/stdio.h>
#include <types.h>

static pok_event_id_t eid;
static int i;

static void *pinger_job() {
  printf("Hello from P1T1!\n");
  for (int j = 1; j <= 5; j++) {
    pok_thread_sleep(10000);
    printf("P1T1: I signal event (signal %d)\n", j);
    pok_event_signal(eid);
    pok_thread_sleep(10000);
    printf("P1T1: I broadcast event (broadcast %d)\n", j);
    pok_event_broadcast(eid);
  }
  printf("P1T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  printf("Hello from P1T2!\n");
  for (;;) {
    pok_event_lock(eid);
    printf("P1T2: I will wait for the signal (round %d)\n", i);
    pok_event_wait(eid, 0);
    printf("P1T2: got the event (round %d)\n", i++);
    pok_event_unlock(eid);
  }
  return NULL;
}

static void *pinger_job3() {
  printf("Hello from P1T3!\n");
  pok_event_unlock(eid);
  for (;;) {
    pok_event_lock(eid);
    printf("P1T3: I will wait for the signal (round %d)\n", i);
    pok_event_wait(eid, 0);
    printf("P1T3: got the event (round %d)\n", i++);
    pok_event_unlock(eid);
  }
  return NULL;
}

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;
  i = 1;

  ret = pok_event_create(&eid);
  printf("[P1] pok_event_create return=%d, mid=%d\n", ret, eid);

  tattr.priority = 40;
  tattr.entry = pinger_job;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (2) return=%d\n", ret);

  tattr.priority = 44;
  tattr.entry = pinger_job3;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (3) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
