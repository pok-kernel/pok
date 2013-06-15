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
 * Created by julien on Wed Feb 18 19:05:30 2009 
 */


#ifndef __POK_SCHEDVALUES_H__
#define __POK_SCHEDVALUES_H__

typedef enum
{
   POK_SCHED_FIFO             = 0,
   POK_SCHED_RR               = 1,
   POK_SCHED_GLOBAL_TIMESLICE = 2,
   POK_SCHED_RMS              = 3,
   POK_SCHED_EDF              = 4,
   POK_SCHED_LLF              = 5,
   POK_SCHED_STATIC           = 6
} pok_sched_t;

#endif

