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

#ifdef POK_CONFIG_NEEDS_FUNC_STRCPY

__attribute__ ((weak))
char		*strcpy(char *dest, const char *str)
{
  unsigned int i;
  for (i = 0; str[i];i++)
    dest[i] = str[i];
  dest[i] = '\0';
  return dest;
}

#endif

#ifdef POK_CONFIG_NEEDS_FUNC_STRNCPY

__attribute__ ((weak))
char *strncpy(char *dest, const char *str, size_t size)
{
  unsigned int i;
  for (i = 0; i < size; i++)
  {
    dest[i] = str[i];
    if (str[i] == '\0')
      break;
  }
  return dest;
}

#endif

