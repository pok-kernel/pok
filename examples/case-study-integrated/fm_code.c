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

/****************************************/
/* user_navigation_sensor_processing   */ 
/***************************************/

void user_navigation_sensor_processing 
    (types__navsignaldata navsignaldatafromsensor,
    types__navsensordata* navsensordatatoin)
{
   printf ("[FM] USER NAVIGATION SENSOR PROCESSING\n");
}


/*********************************/
/* user_integrated_navigation   */ 
/********************************/

void user_integrated_navigation 
    (types__navsensordata navsensordatafromnsp,
    types__navdata* navdatatogpapc)
{
   printf ("[FM] USER INTEGRATED NAVIGATION\n");
}


/*******************************/
/* user_guidance_processing   */ 
/******************************/

void user_guidance_processing 
    (types__navdata navdatafromin,
    types__fpdata fpdatafromfpp,
    types__guidancedata* guidancetofpp)
{
   printf ("[FM] USER GUIDANCE PROCESSING\n");
}


/**********************************/
/* user_flight_plan_processing   */ 
/*********************************/

void user_flight_plan_processing 
    (types__guidancedata guidancefromgp,
    types__performancedata perfdatafromapc,
    types__fpdata* fpdatatogpapc)
{
   printf ("[FM] USER FLIGHT PLAN PROCESSING\n");
}


/********************************************/
/* user_aircraft_performance_calculation   */ 
/*******************************************/

void user_aircraft_performance_calculation 
    (types__fpdata fpdatafromfpp,
    types__navdata navdatafromin,
    types__fuelflowdata fuelflow,
    types__performancedata* perfdatatofpp)
{
   printf ("[FM] USER AIRCRAFT PERFORMANCE CALCULATION\n");
}


/*******************************/
/* user_handle_page_request   */ 
/******************************/

void user_handle_page_request 
    (types__pagerequestcmd new_page_request_from_pcm,
    types__pagecontent* new_page_content_to_pcm,
    types__pagerequestcmd* new_page_request_to_fd,
    types__pagecontent new_page_content_from_fd)
{
   printf ("[FM] USER HANDLE PAGE REQUEST\n");
}


/***********************/
/* user_periodic_io   */ 
/**********************/

void user_periodic_io 
    (types__fuelflowdata fromoutside_fuelflow,
    types__navsignaldata fromoutside_navsignal,
    types__fuelflowdata* fuelflow,
    types__navsignaldata* navsignal,
    types__guidancedata guidanceout,
    types__fpdata fpdataout,
    types__navdata navdataout,
    types__guidancedata* tooutside_guidanceout,
    types__fpdata* tooutside_fpdataout,
    types__navdata* tooutside_navdataout,
    types__pagerequestcmd new_page_request_from_pcm,
    types__pagerequestcmd* new_page_request_to_pagefeed,
    types__pagerequestcmd new_page_request_from_pagefeed,
    types__pagerequestcmd* new_page_request_to_fd,
    types__pagecontent new_page_content_from_fd,
    types__pagecontent* new_page_content_to_pagefeed,
    types__pagecontent new_page_content_from_pagefeed,
    types__pagecontent* new_page_content_to_pcm)
{
   printf ("[FM] USER PERIODIC IO\n");
   printf ("[FM] CONTENT FROM FD=%d\n",new_page_content_from_fd);
}




