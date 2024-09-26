/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2024 POK team
 */

#include <core/debug.h>
#include <core/error.h>
#include <core/partition.h>
#include <core/thread.h>
#include <types.h>

#include <libc.h>

pok_ret_t pok_error_thread_create(uint32_t stack_size, void *entry) {
  uint32_t tid;
  pok_thread_attr_t attr;
  pok_ret_t ret;

  (void)stack_size;

  attr.priority = POK_THREAD_MAX_PRIORITY;
  attr.entry = entry;
  attr.time_capacity = 10000;

  ret = pok_partition_thread_create(&tid, &attr, POK_SCHED_CURRENT_PARTITION);

  POK_CURRENT_PARTITION.thread_error = tid;

  pok_sched_activate_error_thread();

  return (ret);
}

/**
 * Declare the error to the recover process.
 * If the recover process is not created, the partition handler
 * is called.
 */
void pok_error_declare(const uint8_t error) {
  /**
   * Ok, the error handler process is created inside the partition
   * so we declare the error in a appropriate structure.
   */

  if (POK_CURRENT_PARTITION.thread_error != 0) {
    POK_CURRENT_PARTITION.error_status.error_kind = error;
    POK_CURRENT_PARTITION.error_status.failed_thread =
        POK_SCHED_CURRENT_THREAD - POK_CURRENT_PARTITION.thread_index_low;
    POK_CURRENT_PARTITION.error_status.msg_size = 0;
    /*
     * FIXME: Add failed address and so on.
     */
  } else {
    /**
     * If the current partition does not have an error handler process,
     * we raise the error inside the partition.
     */
    pok_partition_error(POK_SCHED_CURRENT_PARTITION, error);
  }
}

void pok_error_ignore() { /* Do nothing at this time */ }

#ifndef POK_USE_GENERATED_KERNEL_ERROR_HANDLER
void pok_kernel_error(uint32_t error) {
#ifdef POK_NEEDS_DEBUG
  printf("[KERNEL] [WARNING] Error %d (%s) was raised by the kernel but no "
         "error recovery was set\n",
         error, pok_error_message(error));
#else
  (void)error;
#endif /* POK_NEEDS_DEBUG */
  return;
}
#endif

#ifndef POK_USE_GENERATED_PARTITION_ERROR_HANDLER
void pok_partition_error(uint8_t partition, uint32_t error) {
#ifdef POK_NEEDS_DEBUG
  const char *error_msg = pok_error_message(error);
  printf("[KERNEL] [WARNING] Error %d (%s) was raised by partition %d but no "
         "error recovery was set\n",
         error, error_msg, partition);
  pok_fatal(error_msg);
#else
  (void)partition;
  (void)error;
#endif /* POK_NEEDS_DEBUG */
  return;
}
#endif /* POK_USE_GENERATED_PARTITION_ERROR_HANDLER */

#ifndef POK_USE_GENERATED_KERNEL_ERROR_CALLBACK
void pok_error_kernel_callback() {
#ifdef POK_NEEDS_DEBUG
  printf("[KERNEL] [WARNING] Kernel calls callback function but nothing was "
         "defined by the user\n");
  printf("[KERNEL] [WARNING] You MUST define the pok_error_partition_callback "
         "function\n");
#endif /* POK_NEEDS_DEBUG */
  return;
}
#endif /* POK_USE_GENERATED_KERNEL_ERROR_CALLBACK */

#ifndef POK_USE_GENERATED_PARTITION_ERROR_CALLBACK
void pok_error_partition_callback(uint32_t partition) {
#ifdef POK_NEEDS_DEBUG
  printf("[KERNEL] [WARNING] Partition %d calls callback function but nothing "
         "was defined by the user\n",
         partition);
  printf("[KERNEL] [WARNING] You MUST define the pok_error_partition_callback "
         "function\n");
#else
  (void)partition;
#endif
  return;
}
#endif /* POK_USE_GENERATED_PARTITION_ERROR_CALLBACK */

void pok_error_raise_application_error(char *msg, uint32_t msg_size) {
  if (msg_size > POK_ERROR_MAX_MSG_SIZE) {
    msg_size = POK_ERROR_MAX_MSG_SIZE;
  }

  pok_error_status_t *status;
  status = &pok_partitions[pok_current_partition].error_status;
  status->error_kind = POK_ERROR_KIND_APPLICATION_ERROR;
  status->failed_thread =
      POK_SCHED_CURRENT_THREAD - POK_CURRENT_PARTITION.thread_index_low;
  status->msg_size = msg_size;

  memcpy(status->msg, msg, msg_size);

  pok_sched_activate_error_thread();
}

pok_ret_t pok_error_get(pok_error_status_t *status) {
  if (POK_CURRENT_PARTITION.error_status.error_kind != POK_ERROR_KIND_INVALID) {

    status->error_kind = POK_CURRENT_PARTITION.error_status.error_kind;
    status->failed_thread = POK_CURRENT_PARTITION.error_status.failed_thread;
    status->failed_addr = POK_CURRENT_PARTITION.error_status.failed_addr;
    status->msg_size = POK_CURRENT_PARTITION.error_status.msg_size;
    memcpy(status->msg, POK_CURRENT_PARTITION.error_status.msg,
           POK_CURRENT_PARTITION.error_status.msg_size);

    return POK_ERRNO_OK;
  } else {
    return POK_ERRNO_UNAVAILABLE;
  }
}

const char *pok_error_message(uint8_t error) {
  switch (error) {
  case POK_ERROR_KIND_INVALID:
    return "invalid";
  case POK_ERROR_KIND_DEADLINE_MISSED:
    return "deadline missed";
  case POK_ERROR_KIND_APPLICATION_ERROR:
    return "application error";
  case POK_ERROR_KIND_NUMERIC_ERROR:
    return "numeric error";
  case POK_ERROR_KIND_ILLEGAL_REQUEST:
    return "illegal request";
  case POK_ERROR_KIND_STACK_OVERFLOW:
    return "stack overflow";
  case POK_ERROR_KIND_MEMORY_VIOLATION:
    return "memory violation";
  case POK_ERROR_KIND_HARDWARE_FAULT:
    return "hardware fault";
  case POK_ERROR_KIND_POWER_FAIL:
    return "power failure";
  case POK_ERROR_KIND_PARTITION_CONFIGURATION:
    return "partition configuration error";
  case POK_ERROR_KIND_PARTITION_INIT:
    return "partition initialization error";
  case POK_ERROR_KIND_PARTITION_SCHEDULING:
    return "partition scheduling error";
  case POK_ERROR_KIND_PARTITION_HANDLER:
    return "partition handler";
  case POK_ERROR_KIND_PARTITION_PROCESS:
    return "partition process";
  case POK_ERROR_KIND_KERNEL_INIT:
    return "kernel initialization error";
  case POK_ERROR_KIND_KERNEL_SCHEDULING:
    return "kernel scheduling error";
  case POK_ERROR_KIND_KERNEL_CONFIG:
    return "kernel configuration error";
  default:
    return "unknown error";
  }
}
