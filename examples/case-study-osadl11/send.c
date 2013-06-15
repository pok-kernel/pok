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
 *                                           Copyright (c) 2011 POK team
 */


#include <types.h>
#include <libc/stdio.h>

int n = 1;

void user_send (int* t)
{
   *t = n;
   printf ("Sent value %d\n", n);
   n++;
}
