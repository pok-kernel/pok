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

#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

#include <arch.h>
#include <errno.h>
#include <core/cons.h>



print_char_fn_t pok_print_char_fn = NULL;
print_attr_fn_t pok_print_attr_fn = NULL;



void pok_print_char (const char c)
{
   if (pok_print_char_fn != NULL)
   {
      return (pok_print_char_fn(c));
   }
}

void pok_print_attr (uint8_t attr)
{
   if (pok_print_attr_fn != NULL)
   {
      pok_print_attr_fn(attr);
   }
}

int pok_write (const char *s, size_t length)
{
   size_t i;

   for (i = 0; i < length; i++)
   {
      pok_print_char (*s++);
   }

   return (i);
}

int pok_print_init (print_char_fn_t print_char_fn,
                    print_attr_fn_t print_attr_fn)
{
   pok_print_char_fn = print_char_fn;
   pok_print_attr_fn = print_attr_fn;

   return (POK_ERRNO_OK);
}

#endif /* POK_NEEDS_CONSOLE, POK_NEEDS_DEBUG */
