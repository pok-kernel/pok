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

#ifndef __POK_KERNEL_SEMAPHORE_H__
#define __POK_KERNEL_SEMAPHORE_H__

#include <core/dependencies.h>

#ifdef POK_NEEDS_SEMAPHORES

#include <core/lockobj.h>
#include <errno.h>
#include <types.h>

pok_ret_t pok_sem_create(pok_sem_id_t *id, const pok_sem_value_t current_value,
                         const pok_sem_value_t max_value,
                         const pok_queueing_discipline_t discipline);

pok_ret_t pok_sem_wait(pok_sem_id_t id, uint64_t timeout_us);

pok_ret_t pok_sem_signal(pok_sem_id_t id);

pok_ret_t pok_sem_id(char *name, pok_sem_id_t *id);

pok_ret_t pok_sem_status(pok_sem_id_t id, pok_sem_status_t *status);

#endif

#endif
