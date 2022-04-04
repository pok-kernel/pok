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

#ifdef POK_NEEDS_MUTEXES
#include <core/lockobj.h>
#include <core/mutex.h>
#include <core/syscall.h>
#include <core/time.h>
#include <errno.h>
#include <types.h>

/*
 * Try to lock the mutex
 * @param id ID of the mutex
 * @param timeout_us Time to wait in microseconds
 */
pok_ret_t pok_mutex_trylock(pok_mutex_id_t id, uint64_t timeout_us) {
  pok_lockobj_lockattr_t lockattr;
  lockattr.operation = LOCKOBJ_OPERATION_LOCK;
  lockattr.timeout = timeout_us * 1000;
  lockattr.obj_kind = POK_LOCKOBJ_KIND_MUTEX;
  return (pok_syscall2(POK_SYSCALL_LOCKOBJ_OPERATION, (uint32_t)id,
                       (uint32_t)&lockattr));
}

#endif
