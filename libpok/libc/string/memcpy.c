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

#include <core/dependencies.h>
#include <libc/string.h>

#if defined (POK_CONFIG_NEEDS_FUNC_MEMCPY) || (defined (POK_ARCH_SPARC) && defined (POK_NEEDS_FUNC_PRINTF))
/*
 * For printf, the SPARC architecture needs memcpy() func
 */

__attribute__ ((weak))
void		*memcpy(void *dest, const void *src, size_t count)
{
  const unsigned char *s = (const unsigned char*) src;
  unsigned char *d = (unsigned char*) dest;

  while (count--)
  {
    *d++ = *s++;
  }

  return (dest);
}


#endif

