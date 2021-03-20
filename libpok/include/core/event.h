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

#ifndef __POK_LIBPOK_EVENT_H__
#define __POK_LIBPOK_EVENT_H__

#include <core/dependencies.h>

#include <errno.h>
#include <types.h>

pok_ret_t pok_event_create(pok_event_id_t *id);
pok_ret_t pok_event_wait(pok_event_id_t id, uint64_t timeout_us);
pok_ret_t pok_event_broadcast(pok_event_id_t id);
pok_ret_t pok_event_signal(pok_event_id_t id);
pok_ret_t pok_event_lock(pok_event_id_t id);
pok_ret_t pok_event_unlock(pok_event_id_t id);

#endif
