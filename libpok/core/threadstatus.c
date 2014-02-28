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
 *                                      Copyright (c) 2007-2012 POK team 
 *
 * Created by matias on Wed Feb 29 14:15:08 2012 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_THREADS
#include <arch.h>
#include <types.h>
#include <core/syscall.h>
#include <core/thread.h>

pok_ret_t pok_thread_status (const uint32_t thread_id, pok_thread_attr_t* attr)
{
  return pok_syscall2  (POK_SYSCALL_THREAD_STATUS,
			(uint32_t)thread_id,
			(uint32_t)attr);
}

#endif
