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

#ifndef __POK_KERNEL_QUEUES_H__
#define __POK_KERNEL_QUEUES_H__

#if defined(POK_NEEDS_PORTS_SAMPLING) || defined(POK_NEEDS_PORTS_QUEUEING)

#include <types.h>

typedef struct {
  char *data;
  uint32_t size;
  uint32_t available_size;
} pok_queue_t;

void pok_queue_init();

#endif

#endif
