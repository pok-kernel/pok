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
    // printf("--- Processor %hhd, Time: %u \n",current_proc,(unsigned)(POK_GETTICK()));
    if(res == IDLE_THREAD){
      printf("Idle at %u.\n",(unsigned)(POK_GETTICK()));
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      printf("P%hhdT%d: Start scheduling (priority: %d) at %u\n",
      current_proc,
      res,
      pok_threads[res].priority,
      (unsigned)(POK_GETTICK()));
    }
    else{
      printf("P%hhdT%d: Remaining time:%u (priority: %d) at %u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity),
      pok_threads[res].priority,
      (unsigned)(POK_GETTICK()));
    }
    // if(res != IDLE_THREAD){
    //   uint32_t first =1;
    //   for (uint32_t i = index_low + 1; i < index_high; i++) {
    //     if (pok_threads[i].state == POK_STATE_RUNNABLE &&
    //         pok_threads[i].processor_affinity == current_proc) {
    //       if (i != elected) {
    //         printf("%s %d (%d)", first ? "    --- other ready: " : ",", i,
    //                pok_threads[i].priority);
    //         first = 0;
    //       }
    //     }
    //   }
    //   if (!first) {
    //     printf("\n");
    //   }
    // }
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
    // printf("--- Processor %hhd, Time: %u \n",current_proc,(unsigned)(POK_GETTICK()));
    if(res == IDLE_THREAD){
      printf("Idle at %u.\n",(unsigned)(POK_GETTICK()));
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      printf("P%hhdT%d: Start scheduling (deadline: %u) at %u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].ab_deadline),
      (unsigned)(POK_GETTICK()));
    }
    else{
      printf("P%hhdT%d: Remaining time:%u (deadline: %u) at %u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity),
      (unsigned)(pok_threads[res].ab_deadline),
      (unsigned)(POK_GETTICK()));
    }
    // if(res != IDLE_THREAD){
    //   uint32_t first =1;
    //   for (uint32_t i = index_low + 1; i < index_high; i++) {
    //     if (pok_threads[i].state == POK_STATE_RUNNABLE &&
    //         pok_threads[i].processor_affinity == current_proc) {
    //       if (i != elected) {
    //         printf("%s %d (%u)", first ? "    --- other ready: " : ",", i,
    //                (unsigned)(pok_threads[i].ab_deadline));
    //         first = 0;
    //       }
    //     }
    //   }
    //   if (!first) {
    //     printf("\n");
    //   }
    // }
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
    // printf("--- Processor %hhd, Time: %u \n",current_proc,(unsigned)(POK_GETTICK()));
    if(res == IDLE_THREAD){
      printf("Idle at %u.\n",(unsigned)(POK_GETTICK()));
    }
    else if(pok_threads[res].remaining_time_capacity == pok_threads[res].time_capacity){
      // printf("Time %u: Start scheduling thread %d\n",
      // (unsigned)POK_GETTICK(),
      // res);
      printf("P%hhdT%d: Start scheduling at %u\n",
      current_proc,
      res,
      (unsigned)(POK_GETTICK()));
    }
    else{
      printf("P%hhdT%d: Remaining time:%u at %u\n",
      current_proc,
      res,
      (unsigned)(pok_threads[res].remaining_time_capacity),
      (unsigned)(POK_GETTICK()));
    }
    // if(res != IDLE_THREAD){
    //   uint32_t first =1;
    //   for (uint32_t i = index_low + 1; i < index_high; i++) {
    //     if (pok_threads[i].state == POK_STATE_RUNNABLE &&
    //         pok_threads[i].processor_affinity == current_proc) {
    //       if (i != elected) {
    //         printf("%s %d", first ? "    --- other ready: " : ",", i);
    //         first = 0;
    //       }
    //     }
    //   }
    //   if (!first) {
    //     printf("\n");
    //   }
    // }
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