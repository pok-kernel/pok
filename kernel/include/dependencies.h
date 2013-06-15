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
 * Created by julien on Sat May 16 14:40:46 2009 
 */

#ifndef __POK_KERNEL_DEPENDENCIES_H__
#define __POK_KERNEL_DEPENDENCIES_H__

#ifdef POK_NEEDS_ERROR_HANDLING
   #ifndef POK_NEEDS_SCHED_STOP_SELF
      #define POK_NEEDS_SCHED_STOP_SELF 1
   #endif
#endif

#ifdef POK_NEEDS_THREAD_SUSPEND
   #ifndef POK_NEEDS_SCHED_STOP_SELF
      #define POK_NEEDS_SCHED_STOP_SELF 1
   #endif
#endif

#endif
