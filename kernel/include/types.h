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

#ifndef __POK_TYPES_H__
#define __POK_TYPES_H__

#ifdef POK_ARCH_SPARC
#include <arch/sparc/types.h>
#elif POK_ARCH == x86
#include <arch/x86/types.h>
#endif

#define NULL 0
#define FALSE 0
#define TRUE 1
#define bool_t int
#define pok_bool_t int

typedef uint32_t pok_port_size_t;
typedef uint8_t pok_port_direction_t;
typedef uint8_t pok_port_kind_t;
typedef uint8_t pok_port_id_t;
typedef uint8_t pok_size_t;
typedef uint8_t pok_range_t;
typedef uint8_t pok_buffer_id_t;
typedef uint8_t pok_blackboard_id_t;
typedef uint8_t pok_lockobj_id_t;
typedef uint8_t pok_sem_id_t;
typedef uint8_t pok_event_id_t;
typedef uint8_t pok_partition_id_t;
typedef uint16_t pok_sem_value_t;

#endif
