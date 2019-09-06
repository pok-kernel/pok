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
 *                                      Copyright (c) 2007-2019 POK team 
 *
 * Created by etienne on Mer Sept 4 23:49:13 2019 
 */


#include <core/dependencies.h>
#include <libc/string.h>

#ifdef POK_CONFIG_NEEDS_FUNC_ATOI

int atoi(const char *str)
{
  int res = 0;
  int i = 0;
  while(str[i]!='\0')
  {
    char c = str[i];
    if(c<48 || c>57)
      return 0;
    res = 10*res + c - 48;
    i++;
  }
  return res;
}

#endif
