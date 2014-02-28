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


#ifndef POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE

#ifdef POK_NEEDS_TIME
#include <errno.h>
#include <types.h>
#include <core/time.h>

pok_ret_t pok_time_compute_deadline (const pok_time_t relative, pok_time_t* absolute)
{
   if (pok_time_gettick (absolute) != POK_ERRNO_OK)
   {
      return (POK_ERRNO_TIME);
   }
   *absolute = *absolute + relative;
   return POK_ERRNO_OK;
}

#endif

#endif /* POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE */

