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
 *                                      Copyright (c) 2007-2022 POK team
 */

/**
 * \file    core/boot.c
 * \author  Julien Delange
 * \brief   Boot function to start the kernel
 * \date    2008-2009
 */

#include <arch.h>
#include <bsp.h>

#include <core/boot.h>
#include <core/partition.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/time.h>
#include <middleware/port.h>
#include <middleware/queue.h>

#include <core/instrumentation.h>

void pok_boot() {
  pok_arch_init();
  pok_bsp_init();
  pok_time_init();
  pok_partition_init();
  pok_thread_init();
  pok_sched_init();

#if (defined POK_NEEDS_LOCKOBJ) || defined(POK_NEEDS_PORTS_QUEUEING) ||        \
    defined(POK_NEEDS_PORTS_SAMPLING)
  pok_lockobj_init();
#endif
#if defined(POK_NEEDS_PORTS_QUEUEING) || defined(POK_NEEDS_PORTS_SAMPLING)
  pok_port_init();
  pok_queue_init();
#endif

#if defined(POK_NEEDS_DEBUG) || defined(POK_NEEDS_CONSOLE)
  pok_cons_write("POK kernel initialized\n", 23);
#endif

#ifdef POK_NEEDS_INSTRUMENTATION
  uint32_t tmp;
  printf("[INSTRUMENTATION][CHEDDAR] <event_table>\n");
  printf("[INSTRUMENTATION][CHEDDAR] <processor>\n");
  printf("[INSTRUMENTATION][CHEDDAR] <name>pok_kernel</name>\n");

  for (tmp = 0; tmp < POK_CONFIG_NB_THREADS; tmp++) {
    printf("[INSTRUMENTATION][CHEDDAR] <task_activation>   0   task "
           "%d</task_activation>\n",
           tmp);
  }
#endif

  pok_arch_preempt_enable();
}
