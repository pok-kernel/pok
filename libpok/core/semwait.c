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

#include <core/dependencies.h>

#ifdef POK_NEEDS_SEMAPHORES
#include <core/lockobj.h>
#include <core/semaphore.h>
#include <core/syscall.h>
#include <errno.h>
#include <types.h>

/*
 * Try to lock to get a ressource from the semaphore
 * @param id ID of the semaphore
 * @param us Time to wait in microseconds
 */
pok_ret_t pok_sem_wait(pok_sem_id_t id, uint64_t timeout_us) {
  pok_lockobj_lockattr_t lockattr;
  lockattr.operation = LOCKOBJ_OPERATION_LOCK;
  lockattr.obj_kind = POK_LOCKOBJ_KIND_SEMAPHORE;
  lockattr.timeout = timeout_us * 1000;
  return (pok_syscall2(POK_SYSCALL_LOCKOBJ_OPERATION, (uint32_t)id,
                       (uint32_t)&lockattr));
}

#endif
