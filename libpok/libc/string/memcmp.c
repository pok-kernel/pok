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
#include <types.h>

#ifdef POK_CONFIG_NEEDS_FUNC_MEMCMP

int memcmp(const void *v1, const void *v2, size_t n) {
  uint32_t *s1;
  uint32_t *s2;
  size_t i;

  s1 = (uint32_t *)v1;
  s2 = (uint32_t *)v2;

  for (i = 0; i < n; i++) {
    if (*s1 != *s2) {
      return *(const uint32_t *)s1 > *(const uint32_t *)s2 ? 1 : -1;
    }
    s1++;
    s2++;
  }
  return 0;
}

#endif
