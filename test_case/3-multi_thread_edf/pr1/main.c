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

#include <core/partition.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <core/time.h>
#include <types.h>

uint8_t sid;

void task() {
  for (;;) {
  }
}

int main() {
  uint32_t tid;
  pok_thread_attr_t tattr;

  tattr.period = 1200;
  tattr.time_capacity = 200;
  tattr.deadline = 400;
  tattr.entry = task;
  pok_thread_create(&tid, &tattr);
  
  tattr.period = 300;
  tattr.time_capacity = 200;
  tattr.deadline = 300;
  tattr.entry = task;
  pok_thread_create(&tid, &tattr);
  
  tattr.period = 400;
  tattr.time_capacity = 200;
  tattr.deadline = 600;
  tattr.entry = task;
  pok_thread_create(&tid, &tattr);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);
  pok_thread_wait_infinite();

  return (0);
}
