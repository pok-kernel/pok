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

#ifndef __POK_LIBPOK_LOCKOBJ_H__
#define __POK_LIBPOK_LOCKOBJ_H__

#include <types.h>

typedef enum {
  POK_LOCKOBJ_KIND_MUTEX = 1,
  POK_LOCKOBJ_KIND_SEMAPHORE = 2,
  POK_LOCKOBJ_KIND_EVENT = 3
} pok_lockobj_kind_t;

typedef enum {
  POK_QUEUEING_DISCIPLINE_FIFO,
  POK_QUEUEING_DISCIPLINE_PRIORITY
} pok_queueing_discipline_t;

typedef enum {
  POK_LOCKOBJ_POLICY_STANDARD = 0,
  POK_LOCKOBJ_POLICY_PIP = 1,
  POK_LOCKOBJ_POLICY_PCP = 2
} pok_locking_policy_t;

typedef struct {
  pok_lockobj_kind_t kind;
  pok_locking_policy_t locking_policy;
  pok_queueing_discipline_t queueing_policy;
  pok_sem_value_t initial_value;
  pok_sem_value_t max_value;
} pok_lockobj_attr_t;

typedef enum {
  LOCKOBK_LOCK_REGULAR = 1,
  LOCKOBJ_LOCK_TIMED = 2
} pok_lockobj_lock_kind_t;

typedef enum {
  LOCKOBJ_OPERATION_LOCK = 1,
  LOCKOBJ_OPERATION_UNLOCK = 2,
  LOCKOBJ_OPERATION_WAIT = 3,
  LOCKOBJ_OPERATION_SIGNAL = 4,
  LOCKOBJ_OPERATION_BROADCAST = 5
} pok_lockobj_operation_t;

typedef struct {
  pok_lockobj_operation_t operation;
  pok_lockobj_kind_t obj_kind;
  pok_lockobj_lock_kind_t lock_kind;
  uint64_t timeout;
} pok_lockobj_lockattr_t;

// FIFO discipline
#define POK_QUEUEING_DISCIPLINE_DEFAULT POK_QUEUEING_DISCIPLINE_FIFO
#define POK_LOCKING_DISCIPLINE_DEFAULT POK_LOCKOBJ_POLICY_STANDARD

#endif
