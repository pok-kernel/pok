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

#include <core/mutex.h>
#include <core/partition.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <types.h>

#ifndef AFFINITY_0
#define AFFINITY_0 0
#endif

#ifndef AFFINITY_1
#define AFFINITY_1 0
#endif

#ifndef AFFINITY_2
#define AFFINITY_2 0
#endif

static pok_mutex_id_t mid;
static int i;
static uint8_t catch0, catch1, catch2 = 0;

static void *pinger_job1() {
  printf("Hello from P2T1!\n");
  for (int j = 1; j <= 5; j++) {
    pok_thread_sleep(2000);
    pok_mutex_lock(mid);
    printf("P2T1: I take mutex (round %d)\n", j);
    catch0++;
    pok_thread_sleep(10000);
    if (catch0 == 5 && catch0 == catch1 && catch0 == catch2)
      printf("SUCCESS!!!\n");
    pok_mutex_unlock(mid);
  }
  printf("P2T1: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job2() {
  printf("Hello from P2T2!\n");
  for (int j = 1; j <= 5; j++) {
    pok_mutex_lock(mid);
    printf("P2T2: I take mutex (round %d)\n", j);
    catch1++;
    pok_thread_sleep(5000);
    if (catch0 == 5 && catch0 == catch1 && catch0 == catch2)
      printf("SUCCESS!!!\n");
    pok_mutex_unlock(mid);
  }
  printf("P2T2: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

static void *pinger_job3() {
  printf("Hello from P2T3!\n");
  for (int j = 1; j <= 5; j++) {
    pok_mutex_lock(mid);
    printf("P2T3: I take mutex (round %d)\n", j);
    catch2++;
    if (catch0 == 5 && catch0 == catch1 && catch0 == catch2)
      printf("SUCCESS!!!\n");
    pok_mutex_unlock(mid);
  }
  printf("P2T3: sleeping forever\n");
  for (;;)
    pok_thread_sleep(1000000);
  return NULL;
}

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;
  i = 1;

  printf("Mutex with FIFO Queueing Discipline\n");

  ret = pok_mutex_create(&mid, POK_QUEUEING_DISCIPLINE_FIFO,
                         POK_LOCKING_DISCIPLINE_DEFAULT);
  printf("[P2] pok_mutex_create return=%d, mid=%d\n", ret, mid);

  tattr.priority = 44;
  tattr.entry = pinger_job1;
  tattr.processor_affinity = AFFINITY_0;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 42;
  tattr.entry = pinger_job2;
  tattr.processor_affinity = AFFINITY_1;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (2) return=%d\n", ret);

  tattr.priority = 40;
  tattr.entry = pinger_job3;
  tattr.processor_affinity = AFFINITY_2;

  ret = pok_thread_create(&tid, &tattr);
  printf("[P2] pok_thread_create (3) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);

  return 0;
}
