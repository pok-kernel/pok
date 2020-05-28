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

#ifndef __POK_LIBC_STDIO_H__
#define __POK_LIBC_STDIO_H__

#include "stdarg.h"

#define EOF -1

int vprintf(const char *format, va_list args);
;

int printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

int getChar();

#endif /* __POK_LIBC_STDIO_H_ */
