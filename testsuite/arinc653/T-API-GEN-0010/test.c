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
 * Created by julien on Mon May  4 18:09:52 2009 
 */

#define POK_NEEDS_ARINC653_PARTITION
#define POK_NEEDS_ARINC653_PROCESS
#include <arinc653/partition.h>

void test()
{
   OPERATING_MODE_TYPE var;
   var = IDLE;
   var = COLD_START;
   var = WARM_START;
   var = NORMAL;
}
