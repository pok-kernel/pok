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
 *                                      Copyright (c) 2007-2025 POK team
 */

#include <core/partition.h>
#include <core/shutdown.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <types.h>

static void *sleeping_thread() {
  // Do not care, they will never start
  return NULL;
}

static const pok_thread_attr_t tattr = {.priority = 40,
                                        .entry = sleeping_thread};

int main() {
  for (int i = 0; i < 3; i++) {
    uint32_t tid;
    pok_ret_t ret = pok_thread_create(&tid, &tattr);
    printf("[P2] pok_thread_create (%d) returned %d\n", i, ret);
  }

  pok_shutdown();
}
