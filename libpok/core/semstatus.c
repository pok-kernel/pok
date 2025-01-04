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

#include <core/dependencies.h>

#ifdef POK_NEEDS_SEMAPHORES
#include <core/semaphore.h>
#include <errno.h>
#include <types.h>

pok_ret_t pok_sem_status(pok_sem_id_t id, pok_sem_status_t *status) {
  (void)id;
  (void)status;
  return POK_ERRNO_UNAVAILABLE;
}

#endif
