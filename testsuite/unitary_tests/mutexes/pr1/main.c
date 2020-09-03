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

#include <core/mutex.h>
#include <core/partition.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <types.h>

static pok_mutex_id_t mid;
static int i;

static void *pinger_job1() {
  printf("Hello from P1T1!\n");
  for (int j = 1; j <= 5; j++) {
    pok_thread_sleep(2000);
    pok_mutex_lock(mid);
    printf("P1T1: I take mutex (round %d)\n", j);
    pok_thread_sleep(10000);
    pok_mutex_unlock(mid);
  }
  printf("P1T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  printf("Hello from P1T2!\n");
  for (int j = 1; j <= 5; j++) {
    pok_mutex_lock(mid);
    printf("P1T2: I take mutex (round %d)\n", j);
    pok_thread_sleep(5000);
    pok_mutex_unlock(mid);
  }
  printf("P1T2: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job3() {
  printf("Hello from P1T3!\n");
  for (int j = 1; j <= 5; j++) {
    pok_mutex_lock(mid);
    printf("P1T3: I take mutex (round %d)\n", j);
    pok_mutex_unlock(mid);
  }
  printf("P1T3: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;
  i = 1;

  printf("Mutex with Priority Queueing Discipline\n");

  ret = pok_mutex_create(&mid, POK_QUEUEING_DISCIPLINE_PRIORITY,
                         POK_LOCKING_DISCIPLINE_DEFAULT);
  printf("[P1] pok_mutex_create return=%d, mid=%d\n", ret, mid);

  tattr.priority = 44;
  tattr.entry = pinger_job1;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (2) return=%d\n", ret);

  tattr.priority = 40;
  tattr.entry = pinger_job3;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (3) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
