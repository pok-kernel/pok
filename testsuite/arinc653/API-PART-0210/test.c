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

#include <arinc653/partition.h>


void test ()
{
   PARTITION_STATUS_TYPE status;
   status.IDENTIFIER = 1;
   status.PERIOD = 1000000000;
   status.DURATION = 5;
   /* FIXME status.LOCKLEVEL */
   status.OPERATING_MODE = IDLE;
   /* FIXME status.STARTCONDITION */
}

