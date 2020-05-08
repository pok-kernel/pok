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

#if defined(POK_NEEDS_ERROR_HANDLING) &&                                       \
    (!defined(POK_USE_GENERATED_ERROR_HANDLER))
#include <core/error.h>
#include <core/partition.h>
#include <core/thread.h>

/*
 * This is a default error handler and it is used
 * when no error handled is defined.
 * Most of the time, the generated code provides
 * its own error handler.
 */

void pok_error_handler_worker() {
  uint32_t thread = 0;
  uint32_t error = 0;

  pok_error_handler_set_ready(&thread, &error);

  while (1) {
    pok_partition_set_mode(POK_PARTITION_STATE_STOPPED);
  }
}

#endif
