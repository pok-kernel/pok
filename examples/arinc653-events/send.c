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
 *                                      Copyright (c) 2007-2021 POK team
 *
 */

#include <libc/stdio.h>
#include <types.h>

#include "aadl_runtime_services.h"

int round = 0;
void user_send (__user_send_context *  context)
{
  round++;
  int i, limit=round%3;
  for(i=0; i<limit; i++) {
    Put_Value(context->output, NULL);
    Send_Output(context->output);
  }
  printf ("I send %d events\n", limit);
}
