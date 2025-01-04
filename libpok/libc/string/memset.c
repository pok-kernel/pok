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
#include <libc/string.h>

#ifdef POK_CONFIG_NEEDS_FUNC_MEMSET

__attribute__((weak)) void *memset(void *dest, unsigned char val,
                                   size_t count) {
  unsigned char *d = (unsigned char *)dest;

  while (count--)
    *d++ = val;

  return dest;
}

#endif
