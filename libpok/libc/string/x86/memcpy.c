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

void		*memcpy(void * to, const void * from, size_t n)
{
  int		d0;
  int		d1;
  int		d2;

  __asm__ __volatile__(
		       "rep ; movsl\n\t"
		       "testb $2,%b4\n\t"
		       "je 1f\n\t"
		       "movsw\n"
		       "1:\ttestb $1,%b4\n\t"
		       "je 2f\n\t"
		       "movsb\n"
		       "2:"
		       : "=&c" (d0), "=&D" (d1), "=&S" (d2)
		       :"0" (n/4), "q" (n),"1" ((long) to),"2" ((long) from)
		       : "memory");
  return (to);
}

