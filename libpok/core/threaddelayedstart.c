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
 *                                      Copyright (c) 2007-2020 POK team
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_THREADS
#include <arch.h>
#include <core/syscall.h>
#include <core/thread.h>
#include <types.h>

pok_ret_t pok_thread_delayed_start(const uint32_t thread_id,
                                   const pok_time_t ms) {
  return pok_syscall2(POK_SYSCALL_THREAD_DELAYED_START, (uint32_t)thread_id,
                      (uint32_t)ms);
}

#endif
