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

#include <core/dependencies.h>

#if ( (! defined (POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE )) || ( defined (POK_NEEDS_FUNCTION_TIME_GET)))

#include <errno.h>
#include <types.h>
#include <core/time.h>

pok_ret_t pok_time_get (pok_time_t* ns)
{
   return (pok_time_gettick (ns));
}

pok_ret_t pok_time_gettick(pok_time_t * value)
{
  return pok_syscall2(POK_SYSCALL_GETTICK,(uint32_t) value, 0);
}
/*
 * Get number of ticks that passed since the system starts
 * Similar to : pok_ret_t   pok_time_gettick (uint64_t* value);
 */


#endif /* POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE */

