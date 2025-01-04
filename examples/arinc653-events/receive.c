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
 *                                      Copyright (c) 2007-2025 POK team
 */

#include <libc/stdio.h>
#include <types.h>

#include "aadl_runtime_services.h"

void user_receive(__user_receive_context *context) {
  uint16_t received_events = 0;
  error_code_t err = Get_Count(context->input, &received_events);
  if (!err)
    printf("I received %d events\n", received_events);
}
