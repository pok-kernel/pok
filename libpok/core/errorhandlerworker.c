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
 *                                      Copyright (c) 2007-2020 POK team
 */

#ifndef POK_USE_GENERATED_ERROR_HANDLER

#include <core/error.h>
#include <core/partition.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <libc/string.h>

/*
 * This is a default error handler and it is used
 * when no error handled is defined.
 * Most of the time, the generated code provides
 * its own error handler.
 */
void pok_error_handler_worker() {
  pok_error_status_t status;
  pok_error_get(&status);

#ifdef POK_NEEDS_DEBUG
  printf("Error %d in partition (default error handler), stopping it\n",
         status.error_kind);
  printf("Failed thread: %d – Failed address: 0x%x", status.failed_thread,
         status.failed_addr);
  // Check if a '\0' terminates the message. If not, we cannot use printf and we
  // will try to allocate memory on the stack to add it, but there is no
  // guarantee we won't make it a double fault if there is no space on the
  // stack.
  char *msg;
  if (*(status.msg + status.msg_size) == '\0') {
    msg = status.msg;
  } else {
    msg = __builtin_alloca(status.msg_size + 1);
    memcpy(msg, status.msg, status.msg_size);
    *(msg + status.msg_size) = '\0';
  }
  printf("Error message: %s\n", msg);
#endif // POK_NEEDS_DEBUG

  pok_partition_set_mode(POK_PARTITION_MODE_STOPPED);
}

#endif // !POK_USE_GENERATED_ERROR_HANDLER