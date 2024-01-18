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
 *                                      Copyright (c) 2007-2024 POK team
 */

#include <core/dependencies.h>
#include <libc/string.h>

#ifdef POK_CONFIG_NEEDS_FUNC_STRLEN

__attribute__((weak)) size_t strlen(const char *s) {
  size_t len = 0;

  while (*s++ != '\0')
    ++len;
  return len;
}

#endif
