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

#include <types.h>
#include <core/thread.h>
#include <core/error.h>
#include <core/debug.h>
#include <core/partition.h>

#include <libc.h>

pok_ret_t pok_error_thread_create (uint32_t stack_size, void* entry)
{
   uint32_t tid;
   pok_thread_attr_t attr;
   pok_ret_t         ret;

   (void) stack_size;

   attr.priority  = POK_THREAD_MAX_PRIORITY;
   attr.entry     = entry;
   attr.time_capacity = 10000;

   ret = pok_partition_thread_create (&tid, &attr, POK_SCHED_CURRENT_PARTITION);

   POK_CURRENT_PARTITION.thread_error = tid;

   pok_sched_activate_error_thread ();

   return (ret);
}


/**
 * Declare the error to the recover process.
 * If the recover process is not created, the partition handler
 * is called.
 */
void pok_error_declare (const uint8_t error)
{
   /**
    * Ok, the error handler process is created inside the partition
    * so we declare the error in a appropriate structure.
    */

   if (POK_CURRENT_PARTITION.thread_error != 0)
   {
      POK_CURRENT_PARTITION.error_status.error_kind    = error;
      POK_CURRENT_PARTITION.error_status.failed_thread = POK_SCHED_CURRENT_THREAD - POK_CURRENT_PARTITION.thread_index_low;
      POK_CURRENT_PARTITION.error_status.msg_size      = 0;
      /*
       * FIXME: Add failed address and so on.
       */
   }
   else
   {
      /**
       * If the current partition does not have an error handler process,
       * we raise the error inside the partition.
       */
      pok_partition_error (POK_SCHED_CURRENT_PARTITION, error);
   }
}

void pok_error_ignore ()
{
   /* Do nothing at this time */
}


#ifndef POK_USE_GENERATED_KERNEL_ERROR_HANDLER
void pok_kernel_error (uint32_t error)
{
#ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] [WARNING] Error %d was raised by the kernel but no error recovery was set\n", error);
#else
   (void) error;
#endif /* POK_NEEDS_DEBUG */
   return;
}
#endif

#ifdef POK_NEEDS_PARTITIONS
#ifndef POK_USE_GENERATED_PARTITION_ERROR_HANDLER
void pok_partition_error (uint8_t partition, uint32_t error)
{
#ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] [WARNING] Error %d was raised by partition %d but no error recovery was set\n", error, partition);
#else
   (void) partition;
   (void) error;
#endif /* POK_NEEDS_DEBUG */
   return;
}
#endif /* POK_USE_GENERATED_PARTITION_ERROR_HANDLER */
#endif /* POK_NEEDS_PARTITIONS */


#ifndef POK_USE_GENERATED_KERNEL_ERROR_CALLBACK
void pok_error_kernel_callback ()
{
#ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] [WARNING] Kernel calls callback function but nothing was defined by the user\n");
   printf ("[KERNEL] [WARNING] You MUST define the pok_error_partition_callback function\n");
#endif /* POK_NEEDS_DEBUG */
   return;
}
#endif /* POK_USE_GENERATED_KERNEL_ERROR_CALLBACK */


#ifdef POK_NEEDS_PARTITIONS
#ifndef POK_USE_GENERATED_PARTITION_ERROR_CALLBACK
void pok_error_partition_callback (uint32_t partition)
{
#ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] [WARNING] Partition %d calls callback function but nothing was defined by the user\n", partition);
   printf ("[KERNEL] [WARNING] You MUST define the pok_error_partition_callback function\n");
#else
   (void) partition;
#endif
   return;
}
#endif /* POK_USE_GENERATED_PARTITION_ERROR_CALLBACK */


void pok_error_raise_application_error (char* msg, uint32_t msg_size)
{
   if (msg_size > POK_ERROR_MAX_MSG_SIZE)
   {
      msg_size = POK_ERROR_MAX_MSG_SIZE;
   }

   pok_error_status_t* status;
   status                  = &pok_partitions[pok_current_partition].error_status;
   status->error_kind      = POK_ERROR_KIND_APPLICATION_ERROR;
   status->failed_thread   = POK_SCHED_CURRENT_THREAD - POK_CURRENT_PARTITION.thread_index_low;
   status->msg_size        = msg_size;

   memcpy (status->msg, msg, msg_size);

   pok_sched_activate_error_thread ();
}

pok_ret_t pok_error_get (pok_error_status_t* status)
{
   if (POK_CURRENT_PARTITION.error_status.error_kind != POK_ERROR_KIND_INVALID)
   {

      status->error_kind       = POK_CURRENT_PARTITION.error_status.error_kind;
      status->failed_thread    = POK_CURRENT_PARTITION.error_status.failed_thread;
      status->failed_addr      = POK_CURRENT_PARTITION.error_status.failed_addr;
      status->msg_size         = POK_CURRENT_PARTITION.error_status.msg_size;
      memcpy (status->msg, POK_CURRENT_PARTITION.error_status.msg, POK_CURRENT_PARTITION.error_status.msg_size);

      return POK_ERRNO_OK;
   }
   else
   {
      return POK_ERRNO_UNAVAILABLE;
   }
}

#endif /* POK_NEEDS_PARTITIONS */

#endif

