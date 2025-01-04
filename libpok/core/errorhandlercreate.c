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

#include <core/error.h>
#include <core/syscall.h>
#include <core/thread.h>
#include <types.h>

void pok_error_handler_worker();

pok_ret_t pok_error_handler_create() {
  uint32_t stack_size;
  stack_size = POK_DEFAULT_STACK_SIZE;
  return (pok_syscall2(POK_SYSCALL_ERROR_HANDLER_CREATE, stack_size,
                       (uint32_t)&pok_error_handler_worker));
}
