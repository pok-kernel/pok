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
 * Created by julien on Wed Jan 21 23:07:15 2009 
 */

#ifdef POK_NEEDS_ERROR_HANDLING
#include <core/error.h>
#include <core/thread.h>
#include <core/partition.h>
#include <core/syscall.h>

pok_ret_t pok_error_handler_set_ready (const pok_error_status_t* status)
{
   return pok_syscall2 (POK_SYSCALL_ERROR_HANDLER_SET_READY,
                        (uint32_t)status, 0);
}

#endif
