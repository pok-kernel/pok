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
 *                                      Copyright (c) 2007-2022 POK team
 */

#include "activity.h"
#include <core/thread.h>
#include <middleware/port.h>
#include <types.h>

void main() {
  uint8_t tid;
  int ret;

  pok_thread_attr_t tattr;

  tattr.priority = 42;
  tattr.entry = pingme_job;

  ret = pok_thread_create(&tid, &tattr);

  pok_thread_sleep(100000000);
}
