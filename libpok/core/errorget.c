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

#include <core/dependencies.h>

#include <core/error.h>
#include <core/syscall.h>
#include <types.h>

pok_ret_t pok_error_get(pok_error_status_t *status) {
  return (pok_syscall2(POK_SYSCALL_ERROR_GET, (uint32_t)status, 0));
}
