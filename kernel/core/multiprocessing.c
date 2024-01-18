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

#include <assert.h>
#include <core/multiprocessing.h>
#include <types.h>

uint64_t partition_processor_affinity[];
uint64_t partition_processor_affinity[] = POK_CONFIG_PROCESSOR_AFFINITY;

uint8_t get_default_proc_real_id(const uint8_t pid) {
  return get_proc_real_id(pid, 0);
}

uint8_t get_proc_real_id(const uint8_t pid, const uint8_t partition_proc_id) {
  uint64_t tmp = partition_processor_affinity[pid];
  uint8_t proc = 0;
  uint8_t counter = 0;
  while (counter <= partition_proc_id) {
    counter += (tmp >> (proc++)) & 1;
    assert(proc <= 64);
  }
  return proc - 1;
}

uint8_t get_proc_partition_id(const uint8_t pid, const uint8_t proc_real_id) {
  uint64_t tmp = partition_processor_affinity[pid];
  uint8_t proc = 0;
  assert((tmp >> proc_real_id) & 1);
  for (int i = 0; i < proc_real_id; i++) {
    proc += (tmp >> i) & 1;
  }
  return proc;
}
