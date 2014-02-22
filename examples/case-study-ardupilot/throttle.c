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
 * Created by julien on Thu Oct 15 13:04:22 2009 
 */

#include <libc/stdio.h>
#include <gtypes.h>

void throttle_simulation (int speed)
{
   printf ("[THROTTLE] received speed=%d\n", speed);
}

