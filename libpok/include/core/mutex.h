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

#ifndef __POK_KERNEL_MUTEX_H__
#define __POK_KERNEL_MUTEX_H__

#include <core/lockobj.h>
#include <errno.h>
#include <types.h>

pok_ret_t pok_mutex_create(pok_mutex_id_t *id,
                           const pok_queueing_discipline_t queueing_discipline,
                           const pok_locking_policy_t locking_policy);
pok_ret_t pok_mutex_lock(pok_mutex_id_t id);
pok_ret_t pok_mutex_trylock(pok_mutex_id_t id, uint64_t timeout_us);
pok_ret_t pok_mutex_unlock(pok_mutex_id_t id);

#endif
