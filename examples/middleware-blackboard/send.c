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
#include <libc/stdio.h>

uint8_t toto = 0;

void user_send (uint8_t* t)
{
   *t = toto; 
   printf ("J'envoie la valeur %d\n", toto);
   toto++;
}
