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


#ifndef __POK_LIBC_STRING_H__
#define __POK_LIBC_STRING_H__

#include <types.h>

char     *itoa(int value, char *buff, int radix);
void     *memcpy(void *dest, const void *src, size_t count);
void     *memset(void *dest, unsigned char val, size_t count);
int		strcmp(const char *s1, const char *s2);
int		strncmp(const char *s1, const char *s2, size_t size);
size_t   strlen(const char *s);
char     *strcpy(char *dest, const char *str);
char		*strncpy(char *dest, const char *str, size_t size);
int      memcmp (const void* v1, const void* v2, size_t n);

/*
** XXX: TO REMOVE
*/
int      streq(char* s1, char* s2);

#endif
