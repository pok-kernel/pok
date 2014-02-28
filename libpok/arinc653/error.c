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


#ifdef POK_NEEDS_ARINC653_ERROR
#include <types.h>
#include <errno.h>
#include <arinc653/types.h>
#include <arinc653/error.h>
#include <core/error.h>
#include <core/syscall.h>
#include <libc/string.h>

   /*
#define HIGHEST_ERROR_CODE_VALUE POWER_FAIL
#define LOWEST_ERROR_CODE_VALUE DEADLINE_MISSED
#define CONVERT_ARINC_TO_POK_ERROR(arinc_error,pok_error) \
               switch (arinc_error ){ \
                  case DEADLINE_MISSED: \
                     *pok_error = POK_ERROR_KIND_DEADLINE_MISSED; \
                     return 1; \
                     break; \
                  case APPLICATION_ERROR: \
                     *pok_error = POK_ERROR_KIND_APPLICATION_ERROR; \
                     return 1; \
                     break; \
                  case NUMERIC_ERROR: \
                     *pok_error = POK_ERROR_KIND_NUMERIC_ERROR; \
                     return 1; \
                     break; \
                  case ILLEGAL_REQUEST: \
                     *pok_error = POK_ERROR_KIND_ILLEGAL_REQUEST; \
                     return 1; \
                     break; \
                  case STACK_OVERFLOW: \
                     *pok_error = POK_ERROR_KIND_STACK_OVERFLOW; \
                     return 1; \
                     break; \
                  case MEMORY_VIOLATION: \
                     *pok_error = POK_ERROR_KIND_MEMORY_VIOLATION; \
                     return 1; \
                     break; \
                  case HARDWARE_FAULT: \
                     *pok_error = POK_ERROR_KIND_HARDWARE_FAULT; \
                     return 1; \
                     break; \
                  case POWER_FAIL: \
                     *pok_error = POK_ERROR_KIND_POWER_FAIL; \
                     return 1; \
                     break; \
                  default: \
                     return 0; \
               }
   */

 #define CONVERT_ERROR_POK_TO_ARINC(pok_error,arinc_error) \
               switch (pok_error){ \
                  case POK_ERROR_KIND_DEADLINE_MISSED: \
                     arinc_error = DEADLINE_MISSED; \
                     break; \
                  case POK_ERROR_KIND_APPLICATION_ERROR: \
                     arinc_error = APPLICATION_ERROR; \
                     break; \
                  case POK_ERROR_KIND_NUMERIC_ERROR: \
                     arinc_error = NUMERIC_ERROR; \
                     break; \
                  case POK_ERROR_KIND_ILLEGAL_REQUEST: \
                     arinc_error = ILLEGAL_REQUEST; \
                     break; \
                  case POK_ERROR_KIND_STACK_OVERFLOW: \
                     arinc_error = STACK_OVERFLOW; \
                     break; \
                  case POK_ERROR_KIND_MEMORY_VIOLATION: \
                     arinc_error = MEMORY_VIOLATION; \
                     break; \
                  case POK_ERROR_KIND_HARDWARE_FAULT: \
                     arinc_error = HARDWARE_FAULT; \
                     break; \
                  case POK_ERROR_KIND_POWER_FAIL: \
                     arinc_error = POWER_FAIL; \
                     break; \
               }


/**
 * At this time, it is implemented to have the same behavior as 
 * RAISE_APPLICATION_ERROR. Should change that in the future
 */
void REPORT_APPLICATION_MESSAGE (MESSAGE_ADDR_TYPE    MESSAGE,
                                 MESSAGE_SIZE_TYPE    LENGTH,
                                 RETURN_CODE_TYPE     *RETURN_CODE )
{
   if (LENGTH > 64)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   pok_error_raise_application_error ((char*) MESSAGE, LENGTH);

   *RETURN_CODE = NO_ERROR;
}

void CREATE_ERROR_HANDLER (SYSTEM_ADDRESS_TYPE  ENTRY_POINT,
                           STACK_SIZE_TYPE      STACK_SIZE,
                           RETURN_CODE_TYPE     *RETURN_CODE)
{
   pok_ret_t core_ret;
   core_ret = pok_syscall2 (POK_SYSCALL_ERROR_HANDLER_CREATE, (uint32_t)STACK_SIZE, (uint32_t)ENTRY_POINT);
   if (core_ret == POK_ERRNO_OK)
   {
      *RETURN_CODE = NO_ERROR;
   }
   else
   {
      /* NEEDS TO COMPLETE WITH THE STANDARD ! */
      *RETURN_CODE = INVALID_CONFIG;
   }
}

void GET_ERROR_STATUS (ERROR_STATUS_TYPE  *ERROR_STATUS,
                       RETURN_CODE_TYPE   *RETURN_CODE )
{
   /**
    * Must be completed later : returns NO_ACTION if nothing
    * was detected or INVALID_CONFIG if the process that
    * calls this function is not the error handler process
    */
   pok_error_status_t   core_status;
   pok_ret_t            core_ret;

   core_ret = pok_error_get (&core_status);

   if (core_ret != POK_ERRNO_OK)
   {
      *RETURN_CODE = NO_ACTION; /* Must be improved later ! */
      return;
   }

   CONVERT_ERROR_POK_TO_ARINC (core_status.error_kind, ERROR_STATUS->ERROR_CODE);
   ERROR_STATUS->LENGTH             = core_status.msg_size;
   memcpy (ERROR_STATUS->MESSAGE, core_status.msg, MAX_ERROR_MESSAGE_SIZE);
   ERROR_STATUS->FAILED_PROCESS_ID  = core_status.failed_thread;
   ERROR_STATUS->FAILED_ADDRESS     = (SYSTEM_ADDRESS_TYPE)core_status.failed_addr;

}

void RAISE_APPLICATION_ERROR (ERROR_CODE_TYPE            ERROR_CODE,
                              MESSAGE_ADDR_TYPE          MESSAGE,
                              ERROR_MESSAGE_SIZE_TYPE    LENGTH,
                              RETURN_CODE_TYPE           *RETURN_CODE)
{
  if (LENGTH > 64)
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   if ( (ERROR_CODE != APPLICATION_ERROR))
   {
      *RETURN_CODE = INVALID_PARAM;
      return;
   }

   pok_error_raise_application_error ((char*) MESSAGE, LENGTH);

   *RETURN_CODE = NO_ERROR;
}

#endif
