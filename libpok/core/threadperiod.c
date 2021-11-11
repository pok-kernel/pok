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
 *                                      Copyright (c) 2007-2021 POK team
 */

#include <core/dependencies.h>

#include <core/syscall.h>
#include <types.h>

pok_ret_t pok_thread_period() {
  return pok_syscall2(POK_SYSCALL_THREAD_PERIOD, (uint32_t)NULL,
                      (uint32_t)NULL);
}
