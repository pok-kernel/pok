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
 * Created by julien on Wed Feb 18 16:22:00 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_THREADS

#include <types.h>
#include <core/thread.h>

pok_ret_t pok_thread_attr_init (pok_thread_attr_t* attr)
{
   if (attr == NULL)
   {
      return POK_ERRNO_EINVAL;
   }

   attr->priority       = POK_THREAD_DEFAULT_PRIORITY;
   attr->entry          = NULL;
   attr->period         = 0;
   attr->deadline       = 0;
   attr->time_capacity  = 0;

   return POK_ERRNO_OK;
}

#endif

