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

#ifndef __POK_SCHEDVALUES_H__
#define __POK_SCHEDVALUES_H__

typedef enum {
  POK_SCHED_FIFO = 0,
  POK_SCHED_RR = 1,
  POK_SCHED_GLOBAL_TIMESLICE = 2,
  POK_SCHED_RMS = 3,
  POK_SCHED_EDF = 4,
  POK_SCHED_LLF = 5,
  POK_SCHED_STATIC = 6,

  POK_LAB_SCHED_RR = 12,
  POK_LAB_SCHED_WRR = 13,
  POK_LAB_SCHED_PRI = 14,
  POK_LAB_SCHED_EDF = 15
} pok_sched_t;

#endif
