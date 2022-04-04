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

#include <core/dependencies.h>

#include <arch.h>
#include <core/syscall.h>
#include <core/thread.h>
#include <types.h>

pok_ret_t pok_thread_resume(const uint32_t thread_id) {
  return pok_syscall2(POK_SYSCALL_THREAD_RESUME, (uint32_t)thread_id, 0);
}
