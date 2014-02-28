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
 * Created by julien on Mon Jan 19 10:51:40 2009 
 */

#include <core/dependencies.h>

#ifdef POK_NEEDS_ERROR_HANDLING

#include <types.h>
#include <errno.h>

#define POK_ERROR_MAX_LOGGED 100


typedef struct
{
   uint8_t        error_kind;
   uint32_t       failed_thread;
   uint32_t       failed_addr;
   char*          msg;
   uint32_t       msg_size;
}pok_error_status_t;


typedef struct
{
   uint32_t    thread;
   uint32_t    error;
   pok_time_t  when;
}pok_error_report_t;

extern   pok_error_report_t pok_error_reported[POK_ERROR_MAX_LOGGED];

#define POK_ERROR_KIND_DEADLINE_MISSED          10
#define POK_ERROR_KIND_APPLICATION_ERROR        11
#define POK_ERROR_KIND_NUMERIC_ERROR            12
#define POK_ERROR_KIND_ILLEGAL_REQUEST          13
#define POK_ERROR_KIND_STACK_OVERFLOW           14
#define POK_ERROR_KIND_MEMORY_VIOLATION         15
#define POK_ERROR_KIND_HARDWARE_FAULT           16
#define POK_ERROR_KIND_POWER_FAIL               17
#define POK_ERROR_KIND_PARTITION_CONFIGURATION  30
#define POK_ERROR_KIND_PARTITION_INIT           31
#define POK_ERROR_KIND_PARTITION_SCHEDULING     32
#define POK_ERROR_KIND_PARTITION_PROCESS        33
#define POK_ERROR_KIND_KERNEL_INIT              50
#define POK_ERROR_KIND_KERNEL_SCHEDULING        51

pok_ret_t pok_error_handler_create ();
void pok_error_ignore  (const uint32_t error_id, const uint32_t thread_id);
void pok_error_confirm (const uint32_t error_id, const uint32_t thread_id);
pok_ret_t pok_error_handler_set_ready (const pok_error_status_t*);

void pok_error_log (const uint32_t error_id, const uint32_t thread_id);

void pok_error_raise_application_error (char* msg, uint32_t msg_size);

/**
 * pok_error_get returns POK_ERRNO_OK if the error pointer
 * was registered and an error was registered.
 * It also returns POK_ERRNO_UNAVAILABLE if the pointer
 * was not registered or if nothing was detected
 */
pok_ret_t pok_error_get (pok_error_status_t* status);

#endif

