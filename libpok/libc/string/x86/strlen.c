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


#include <libc/string.h>

size_t                  strlen(const char *s)
{
   int                 d0;
   register int        __res;

   __asm__ __volatile__(
         "repne\n\t"
         "scasb\n\t"
         "notl %0\n\t"
         "decl %0"
         :"=c" (__res), "=&D" (d0) :"1" (s),"a" (0), "0" (0xffffffff));
   return __res;
}

