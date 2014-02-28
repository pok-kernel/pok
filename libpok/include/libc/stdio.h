/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#ifndef __POK_LIBC_STDIO_H__
#define __POK_LIBC_STDIO_H__

#include <stdarg.h>

int		vprintf(const char* format, va_list args);

int		printf(const char *format, ...);


#endif /* __POK_LIBC_STDIO_H_ */
