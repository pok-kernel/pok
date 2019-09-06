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
 * Created by etienne on Wed Sept 04 15:15:15 2019 
 */

#include <stdio.h>
#include <core/syscall.h>

#if defined (POK_CONFIG_NEEDS_FUNC_GETCHAR)
int getChar()
{
  
  char res=-1;
  pok_syscall1 (POK_SYSCALL_GETCHAR, (uint32_t)&res);
  return (int) res;
}
#endif
