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

#ifdef POK_CONFIG_NEEDS_FUNC_ITOA

__attribute__ ((weak))
char		*itoa(int value, char *buff, int radix)
{
  char temp[16];
  int i = 0;
  int j = 0;
  unsigned int digit;
  unsigned int uvalue;

  if ((radix == 10) && (value < 0))
  {
    uvalue = -value;
    buff[j++] = '-';
  }
  else
    uvalue = value;

  do
  {
    digit = uvalue % radix;
    uvalue = (uvalue - digit)/radix;
    if (digit < 10)
      temp[i++] = (char)('0' + digit);
    else
      temp[i++] = (char)('a' + (digit - 10));
  }
  while (uvalue != 0);
  --i;
  do
    buff[j++] = temp[i--];
  while (i >= 0);
  buff[j] = '\0';
  return buff;
}

#endif

