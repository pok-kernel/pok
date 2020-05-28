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

#ifndef __POK_KERNEL_LIBC_H__
#define __POK_KERNEL_LIBC_H__

#include <types.h>

void *memcpy(void *to, const void *from, size_t n);

/* avoid errors for windows */
/*__attribute__ ((weak))*/
void *memset(void *dest, unsigned char val, size_t count);
size_t strlen(const char *str);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t size);

#if defined(POK_NEEDS_CONSOLE) || defined(POK_NEEDS_DEBUG) ||                  \
    defined(POK_NEEDS_INSTRUMENTATION) || defined(POK_NEEDS_COVERAGE_INFOS)
int printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
#endif /* NEEDS_CONSOLE or NEEDS_DEBUG */

#endif
