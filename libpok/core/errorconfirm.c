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

#include <core/error.h>
#include <types.h>

#ifdef POK_NEEDS_DEBUG
#include <libc/stdio.h>
#endif

void pok_error_confirm(const uint32_t error_id, const uint32_t thread_id) {
  pok_error_log(error_id, thread_id);
  return;
}
