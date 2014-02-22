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

#ifdef POK_NEEDS_ERROR_HANDLING

#ifndef __POK_CORE_ERROR_H__
#define __POK_CORE_ERROR_H__

#include <types.h>
#include <core/sched.h>

#define POK_ERROR_KIND_INVALID                   9
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
#define POK_ERROR_KIND_PARTITION_HANDLER        33
#define POK_ERROR_KIND_PARTITION_PROCESS        34
#define POK_ERROR_KIND_KERNEL_INIT              50
#define POK_ERROR_KIND_KERNEL_SCHEDULING        51
#define POK_ERROR_KIND_KERNEL_CONFIG            52

#define POK_ERROR_MAX_MSG_SIZE                  250

typedef struct
{
   uint8_t        error_kind;
   uint32_t       failed_thread;
   uint32_t       failed_addr;
   char           msg[POK_ERROR_MAX_MSG_SIZE];
   uint32_t       msg_size;
} pok_error_status_t;


pok_ret_t   pok_partition_error_set_ready (pok_error_status_t*);
void        pok_error_ignore ();
void        pok_error_declare (const uint8_t error);
pok_ret_t   pok_error_thread_create (uint32_t stack_size, void* entry);
void        pok_partition_error (uint8_t partition, uint32_t error);
void        pok_kernel_error (uint32_t error);
void        pok_error_partition_callback (uint32_t partition);
void        pok_error_kernel_callback ();

pok_ret_t   pok_error_handler_create (void* entry, uint32_t stack_size);


void        pok_error_raise_application_error (char* msg, uint32_t msg_size);
pok_ret_t   pok_error_get (pok_error_status_t* status);
#define POK_ERROR_CURRENT_PARTITION(error) pok_partition_error(pok_current_partition, error);

#endif

#endif
