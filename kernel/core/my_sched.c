#include <libc.h>
#include <core/my_sched.h>
#include <core/time.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/partition.h>

extern pok_thread_t pok_threads[];

uint32_t pok_my_sched_part_prio(const uint32_t index_low, const uint32_t index_high,
                           const uint32_t prev_thread,
                           const uint32_t current_thread) {
  uint32_t elected, from;
  uint8_t current_proc = pok_get_proc_id();
  uint32_t res = IDLE_THREAD;
  from = current_thread == IDLE_THREAD? prev_thread:current_thread;
  elected = from;
  do {
      if (pok_threads[elected].state == POK_STATE_RUNNABLE && 
      pok_threads[elected].priority>pok_threads[res].priority) {
            res = elected;
        }
        elected++;
        if (elected >= index_high) {
          elected = index_low;
        }
    } while (elected != from);
  #ifdef POK_NEEDS_DEBUG
    if(res == IDLE_THREAD){
      printf("P%hhd: It's idle now.\n",current_proc);
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      // printf("Time %u: Start scheduling thread %d\n",
      // (unsigned)POK_GETTICK(),
      // res);
      printf("P%hhdT%d: Start scheduling\n",
      current_proc,
      res);
    }
    else{
      // printf("Time %u: Scheduling thread %d,remaining time:%u(processor:%hhd)\n",
      // (unsigned)POK_GETTICK(),
      // res,
      // (unsigned)(pok_threads[res].remaining_time_capacity),
      // current_proc);
      printf("P%hhdT%d: Remaining time:%u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity));
    }

  #endif
  return res;
}

uint32_t pok_my_sched_part_edf(const uint32_t index_low, const uint32_t index_high,
                           const uint32_t prev_thread,
                           const uint32_t current_thread) {
  uint32_t elected, from;
  uint8_t current_proc = pok_get_proc_id();
  uint32_t res = IDLE_THREAD;
  from = current_thread == IDLE_THREAD? prev_thread:current_thread;
  elected = from;
  do {
      if (pok_threads[elected].state == POK_STATE_RUNNABLE
      &&pok_threads[elected].ab_deadline<pok_threads[res].ab_deadline) {
            res = elected;
        }
        elected++;
        if (elected >= index_high) {
          elected = index_low;
        }
    } while (elected != from);
  #ifdef POK_NEEDS_DEBUG
    if(res == IDLE_THREAD){
      printf("P%hhd: It's idle now.\n",current_proc);
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      // printf("Time %u: Start scheduling thread %d\n",
      // (unsigned)POK_GETTICK(),
      // res);
      printf("P%hhdT%d: Start scheduling at %u, ddl: %u\n",
      current_proc,
      res,
      (unsigned)(POK_GETTICK()),
      (unsigned)(pok_threads[res].ab_deadline));
    }
    else if(pok_threads[res].remaining_time_capacity == 0){
      printf("P%hhdT%d: Finish at %u, ddl: %u\n",
      current_proc,
      res,
      (unsigned)(POK_GETTICK()),
      (unsigned)(pok_threads[res].ab_deadline));
    }
    else{
      // printf("Time %u: Scheduling thread %d,remaining time:%u(processor:%hhd)\n",
      // (unsigned)POK_GETTICK(),
      // res,
      // (unsigned)(pok_threads[res].remaining_time_capacity),
      // current_proc);
      printf("P%hhdT%d: Remaining time:%u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity));
    }

  #endif
  return res;
}

uint32_t my_rr(const uint32_t index_low, const uint32_t index_high,
                           const uint32_t prev_thread,
                           const uint32_t current_thread, const bool_t is_wrr) {
  
  
  uint32_t elected, from;
  uint8_t current_proc = pok_get_proc_id();
  uint32_t res = IDLE_THREAD;

  from = current_thread == IDLE_THREAD? prev_thread:current_thread;
  elected = from;
  if (elected != IDLE_THREAD && 
  (pok_threads[elected].state == POK_STATE_RUNNABLE)&&
  pok_threads[elected].remaining_round > 0){
    res = elected;  
    pok_threads[elected].remaining_round--; 
  }
  else{
    do {
      elected++;
      if (elected >= index_high) {
        elected = index_low;
      }
      if(pok_threads[elected].state == POK_STATE_RUNNABLE){
        res = elected;
        break;
      }
    } while (elected != from);
    if(res != IDLE_THREAD){
      if(is_wrr){
        pok_threads[res].remaining_round = POK_LAB_SCHED_ROUND*pok_threads[res].weight;
      }
      else{
        pok_threads[res].remaining_round = POK_LAB_SCHED_ROUND;
      }
      pok_threads[res].remaining_round --;
    }
  }
  #ifdef POK_NEEDS_DEBUG
    if(res == IDLE_THREAD){
      printf("P%hhd: It's idle now.\n",current_proc);
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      // printf("Time %u: Start scheduling thread %d\n",
      // (unsigned)POK_GETTICK(),
      // res);
      printf("P%hhdT%d: Start scheduling.\n",
      current_proc,
      res);
    }
    else{
      // printf("Time %u: Scheduling thread %d,remaining time:%u(processor:%hhd)\n",
      // (unsigned)POK_GETTICK(),
      // res,
      // (unsigned)(pok_threads[res].remaining_time_capacity),
      // current_proc);
      printf("P%hhdT%d: Remaining time:%u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity));
    }
  #endif
  return res;
}

uint32_t pok_my_sched_part_rr(const uint32_t index_low, const uint32_t index_high,
                           const uint32_t prev_thread,
                           const uint32_t current_thread) {
  return my_rr(index_low,index_high,prev_thread,current_thread,FALSE);
                           }
uint32_t pok_my_sched_part_wrr(const uint32_t index_low, const uint32_t index_high,
                           const uint32_t prev_thread,
                           const uint32_t current_thread) {
  return my_rr(index_low,index_high,prev_thread,current_thread,TRUE); 
                           }