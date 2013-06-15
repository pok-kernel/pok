/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Fri Sep 25 20:46:54 2009 
 */

#ifndef __POK_KERNEL_INSTRUMENTATION_H__
#define __POK_KERNEL_INSTRUMENTATION_H__

#ifdef POK_NEEDS_INSTRUMENTATION

#include <libc.h>
#include <types.h>

void pok_instrumentation_task_archi (const uint32_t id);

void pok_instrumentation_partition_archi (const uint8_t id);

void pok_instrumentation_running_task (const uint32_t id);

#endif /* POK_NEEDS_INSTRUMENTATION */

#endif /* POK_KERNEL_INSTRUMENTATION_H__ */
