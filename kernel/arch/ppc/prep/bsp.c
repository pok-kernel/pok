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

#include "cons.h"
#include <arch.h>
#include <errno.h>

int pok_bsp_init(void) {
  pok_cons_init();

  return (POK_ERRNO_OK);
}

extern char _end[];

static char *heap_end = _end;

void *pok_bsp_mem_alloc(size_t sz) {
  char *res;

  res = (char *)(((unsigned int)heap_end + 4095) & ~4095);
  heap_end = res + sz;
  return res;
}
