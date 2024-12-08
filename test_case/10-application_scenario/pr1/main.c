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

int getInput() {
  int ch;
  while ((ch = getChar()) == -1);
  return ch;
}

void task() {
  for (;;) {
  }
}

int add_task(uint64_t period, uint64_t time_capacity, uint8_t pri) {
  uint32_t tid;
  pok_thread_attr_t tattr;

  tattr.period = period;
  tattr.priority = pri;
  tattr.time_capacity = time_capacity;
  tattr.user_add = TRUE;
  tattr.entry = task;

  pok_ret_t ret;
  ret = pok_thread_create(&tid, &tattr);
  if (ret == POK_ERRNO_OK) {
      printf("Create New Thread: %u\n", (unsigned)tid);
  } else if (ret == POK_ERRNO_TOOMANY) {
      printf("Error: Exceed Thread Number Limit.\n");
  } else {
      printf("Other Error.\n");
  }
  return ret == POK_ERRNO_OK ? tid : (uint32_t)-1;
}

void user_thread() {
  uint32_t tids[20];
  int current_new_thread = 0;

  while (1) {
    printf("0 => Image Recognition\n");
    printf("1 => Chatbot\n");
    printf("2 => Meeting Minutes\n");
    printf("3 => Show New Created Task Information\n");
    printf("4 => Exit\n");
    printf("Enter a number: ");
    
    int number = getInput();

    printf("%d\n", number-'0');
    printf("You choose: %d => ", number-'0');

    uint32_t new_tid;

    if (number == '0') {
      printf("Create Image Recognition Task\n");
      new_tid = add_task(400, 50, 39);
      if (new_tid!=(uint32_t)-1) {
        tids[current_new_thread] = new_tid;
        current_new_thread++;
      }
    } else if (number == '1') {
      printf("Create Chatbot Task\n");
      new_tid = add_task(800, 100, 38);
      if (new_tid!=(uint32_t)-1) {
        tids[current_new_thread] = new_tid;
        current_new_thread++;
      }
    } else if (number == '2') {
      printf("CreatevMeeting Minutes Task\n");
      new_tid = add_task(100, 1000, 37);
      if (new_tid!=(uint32_t)-1) {
        tids[current_new_thread] = new_tid;
        current_new_thread++;
      }
    } 
    else if (number == '3') {
      printf("-----Show Task Information-----\n");
      pok_thread_attr_t showattr;
      for (int i = 0; i < current_new_thread; i++) {
        pok_thread_status(tids[i], &showattr);
        printf("Task Num = %d, Total Num = %d, Finish Number = %d, \
                Miss Num = %d, Throughput = %d, Goodput = %d\n", tids[i], 
                showattr.total_num, showattr.finish_num, showattr.miss_num, 
                showattr.finish_num/showattr.total_num,
                (showattr.finish_num-showattr.miss_num)/showattr.total_num);
      }
      printf("-----New Task Information-----\n");
    } else if (number == '4') {
      printf("-----User Print Exit!-----\n");
      for (int i = 0; i < current_new_thread; i++) {
        pok_thread_suspend_target(tids[i]);
        printf("Stop Thread %d\n", tids[i]);
      }
      printf("-----Stop All Threads!-----\n");
      break;
    }
  }
  printf("Exit!\n");
  pok_thread_wait_infinite();
}


int main() {
  uint32_t tid;
  pok_thread_attr_t tattr;

  tattr.period = -1;
  tattr.period = 40;
  tattr.priority = 40;
  tattr.entry = user_thread;
  pok_thread_create(&tid, &tattr);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);
  pok_thread_wait_infinite();

  return (0);
}
