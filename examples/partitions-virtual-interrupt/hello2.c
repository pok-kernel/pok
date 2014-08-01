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

#include <libc/stdio.h>
#include <libc/stdlib.h>

void user_hello_part2 ()
{
   int a;
   int b = 10;
   int c;
   uint8_t* alloc1;
   uint8_t* alloc2;

   for (c = 0 ; c < 65000 ; c++)
   {
      a = 0 + 1 * b;
      b = b + 1;

      b = b - 2;
      b = b + 1;
      a++;
   }

   alloc1 = malloc (1000);
   alloc2 = calloc (1000, 1);
   printf("[PART2] First memory allocation, adress=0x%x\n", alloc1);
   printf("[PART2] Second memory allocation, adress=0x%x\n", alloc2);
   printf("[PART2] Hello World\n");
   free (alloc1);
   printf("[PART2] Deallocate first memory allocation\n");
   free (alloc2);
   printf("[PART2] Deallocate second memory allocation\n");
}


