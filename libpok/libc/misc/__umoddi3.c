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
 * Created by Etienne on Thu Sept 19 21:14:13 2019 
 */

#include <core/dependencies.h>

#ifdef  POK_CONFIG_NEEDS_FUNC_UMODDI3

unsigned long long __umoddi3 (unsigned long long num,
			                     unsigned long long den)
{
  unsigned long long res = num-(num/den*den);
  return res;
}

#endif
