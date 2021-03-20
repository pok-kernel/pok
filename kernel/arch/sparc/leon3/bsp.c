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
 *                                      Copyright (c) 2007-2021 POK team
 */

/**
 * @file
 * @author Fabien Chouteau
 */

#include "cons.h"
#include "sparc_conf.h"
#include <arch.h>
#include <core/debug.h>
#include <errno.h>

extern char _end[];

static char *heap_end = _end;

int pok_bsp_init(void) {
  pok_cons_init();
  return (POK_ERRNO_OK);
}

/**
 * Used for partition allocation.
 * For SPARC support, all partitions are aligned on page size
 * and all partition sizes have to be less than page size.
 * @see SPARC_PAGE_SIZE
 */
void *pok_bsp_mem_alloc(size_t sz) {
  char *res;

  /* Aligned on page size */
  res =
      (char *)(((uint32_t)heap_end + SPARC_PAGE_SIZE) & ~(SPARC_PAGE_SIZE - 1));
  heap_end = res + sz;
  return res;
}
