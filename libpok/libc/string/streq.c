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
#include <types.h>

#ifdef POK_CONFIG_NEEDS_FUNC_STREQ

/*
** XXX: This is really crap !
**
** Please use strcmp instead.
*/
int streq (char* s1, char* s2)
{
   char* s11 = s1;
   char* s22 = s2;

   uint8_t minlen = strlen (s1) < strlen (s2) ? strlen (s1) : strlen (s2);
   while( ((s11 != NULL) && (s22 != NULL)) && (minlen > 0))
   {
      if (*s11 != *s22)
      {
         return 0;
      }
      minlen--;
      s11++;
      s22++;
   }

   return 1;
}

#endif

