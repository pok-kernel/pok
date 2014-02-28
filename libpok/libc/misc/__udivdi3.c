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

#ifdef  POK_CONFIG_NEEDS_FUNC_UDIVDI3

unsigned long long __udivdi3(unsigned long long num,
			     unsigned long long den)
{
  unsigned long long quot = 0, qbit = 1;

  if (den == 0)
    return 0;

  while ((long long) den >= 0)
  {
    den <<= 1;
    qbit <<= 1;
  }

  while (qbit)
  {
    if (den <= num)
    {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }

  return quot;
}
#endif

