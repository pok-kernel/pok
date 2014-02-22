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
 * Created by julien on Tue Sep 22 13:45:12 2009 
 */

#include <gtypes.h>
#include <libc/stdio.h>

void user_produce_integer (int* a)
{
   static int value = 0;
   *a = value++;
   printf ("I produce integer %d\n", *a);
}

void user_print_two_integer (int one, int two)
{
   printf ("[TOPSECRET] I print first  integer %d\n", one);
   printf ("[SECRET] I print second integer %d\n", two);
}

void user_print_integer (int n)
{
   printf ("[UNCLASSIFIED] I print integer %d\n", n); 
}
