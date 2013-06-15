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
 * Created by julien on Thu Dec  3 10:21:25 2009 
 */

#include <gtypes.h>
#include <libc/stdio.h>

/**********************/
/* user_mfd_driver   */ 
/*********************/

void user_mfd_driver 
    (types__pagerequestcmd* displaytodm_request,
    types__pagecontent displaytodm_page)
{
   printf ("[MFD] USER MFD DRIVER\n");
}


