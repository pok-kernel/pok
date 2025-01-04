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

#ifdef POK_CONFIG_NEEDS_FUNC_RAND

#include <libc/stdlib.h>
#include <types.h>

static uint32_t next = 1;

int rand() {
  /* LINTED integer overflow */
  return (int)((next = next * 1103515245 + 12345) % ((uint32_t)RAND_MAX + 1));
}
#endif
