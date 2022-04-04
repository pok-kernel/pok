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
#include <errno.h>
#include <types.h>

pok_ret_t pok_mutex_create(pok_mutex_id_t *id,
                           const pok_queueing_discipline_t queueing_discipline,
                           const pok_locking_policy_t locking_policy) {
  pok_lockobj_attr_t lockattr;

  lockattr.kind = POK_LOCKOBJ_KIND_MUTEX;
  lockattr.queueing_policy = queueing_discipline;
  lockattr.locking_policy = locking_policy;

  return pok_syscall2(POK_SYSCALL_LOCKOBJ_CREATE, (uint32_t)id,
                      (uint32_t)&lockattr);
}

#endif
