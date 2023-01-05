#include <types.h>

uint32_t pok_my_sched_part_prio(const uint32_t, const uint32_t,
                               const uint32_t prev_thread,
                               const uint32_t current_thread);
uint32_t pok_my_sched_part_edf(const uint32_t, const uint32_t,
                               const uint32_t prev_thread,
                               const uint32_t current_thread);
uint32_t pok_my_sched_part_rr(const uint32_t, const uint32_t,
                               const uint32_t prev_thread,
                               const uint32_t current_thread);
uint32_t pok_my_sched_part_wrr(const uint32_t, const uint32_t,
                               const uint32_t prev_thread,
                               const uint32_t current_thread);