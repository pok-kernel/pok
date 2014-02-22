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
 * Created by julien on Wed Aug 12 15:48:00 2009 
 */

#include <gtypes.h>
#include <libc/stdio.h>

/*

void user_increment_shaded  (sigada09__shaded_i* mydata)
{
   printf("Increment shaded, value before increment   : %d\n", *mydata);
   *mydata = *mydata + 1;
   printf("Increment shaded, value after  increment   : %d\n", *mydata);
}
*/

void user_increment_black (sigada09__black_i* mydata)
{
   printf("Increment black, value before increment    : %d\n", *mydata);
   *mydata = *mydata + 1;
   printf("Increment black, value after  increment    : %d\n", *mydata);
}

void user_print_infos ()
{
   printf("Informations from the infos thread\n");
}

/*
void user_decrement_shaded_and_black (sigada09__shaded_i* shaded_data, sigada09__black_i* black_data)
{
   printf("Decrement shaded, value before increment   : %d\n", *shaded_data);
   *shaded_data = *shaded_data - 1;
   printf("Decrement shaded, value after  increment   : %d\n", *shaded_data);

   printf("Decrement black, value before increment    : %d\n", *black_data);
   *black_data = *black_data - 1;
   printf("Decrement black, value after  increment    : %d\n", *black_data);
}
*/


void user_play_black (sigada09__black_i* mydata)
{
   printf("Play with black data, value before         : %d\n", *mydata);
}


