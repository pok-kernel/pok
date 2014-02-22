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


#include <types.h>
#include <arinc653/error.h>
#include <arinc653/types.h>
#include <libc/stdio.h>

int infinite_loop (int i)
{
   return (infinite_loop (i + 1));
}

int *tmp;
uint32_t cs;
uint32_t ds;
RETURN_CODE_TYPE ret;

void user_receive (int t)
{

   float angle = 32;
   float answer;
   int a = 42;

   asm ("movl %%cs, %0"
         :"=r"(cs));

   asm ("movl %%ds, %0"
         :"=r"(ds));

   printf ("[PART2] Receive value %d CS=%d, DS=%d\n", t, cs, ds);


   switch (t)
   {
      case 2:
         a = t / 0;
         break;

      case 6:
         asm ("fsin" : "=t" (answer) : "0" (angle));
         a = (int) answer;
         break;


      case 8:
         tmp = (int*) 0x2234234;
         a = *tmp;
         break;

      case 12:
         RAISE_APPLICATION_ERROR (APPLICATION_ERROR, (MESSAGE_ADDR_TYPE)"APPERR", 6, &ret);
         break;

      case 16:
         /*
            a = infinite_loop (1);
          * Will create a stack overflow
          */
         break;

      case 18:
         pok_do_syscall (121323123, 0, 0);
         break;

      default:
         a = t;
         break;
   }

   printf ("[PART2] Value computed %d\n", a);
}
