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

#ifdef POK_CONFIG_NEEDS_FUNC_STRCMP

__attribute__ ((weak))
int strcmp(const char *s1, const char *s2)
{
  unsigned int i;
  for (i = 0; ; i++)
  {
    if (s1[i] == '\0' && s2[i] == '\0')
      return 0;
    if (s1[i] < s2[i])
      return -1;
    if (s1[i] > s2[i])
      return 1;
  }
}

#endif 


#ifdef POK_CONFIG_NEEDS_FUNC_STRNCMP

__attribute__ ((weak))
int strncmp(const char *s1, const char *s2, size_t size)
{
  unsigned int i;
  for (i = 0; i < size; i++)
  {
    if (s1[i] == '\0' && s2[i] == '\0')
      return 0;
    if (s1[i] < s2[i])
      return -1;
    if (s1[i] > s2[i])
      return 1;
  }
  return 0;
}

#endif


