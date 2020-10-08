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

/**
 * \file    kernel/include/core/multiprocessing.h
 * \author  Romain Guilloteau
 * \date    2020
 * \brief   Generic interface to handle multiprocessing
 */

#ifndef __POK_MULTIPROCESSING_H__
#define __POK_MULTIPROCESSING_H__
#include <types.h>

extern uint8_t multiprocessing_system;
uint8_t pok_get_proc_id(void);
void pok_send_global_schedule_thread(void);
void pok_send_schedule_thread(uint8_t dest);
void pok_send_schedule_thread_other_processors(void);

uint8_t get_default_proc_real_id(const uint8_t pid);
uint8_t get_proc_real_id(const uint8_t pid, const uint8_t partition_proc_id);
uint8_t get_proc_partition_id(const uint8_t pid, const uint8_t proc_real_id);
#ifdef POK_ARCH_PPC
#endif

#ifdef POK_ARCH_X86
#include <arch/x86/multiprocessing.h>
#endif

#ifdef POK_ARCH_SPARC
#endif

#endif /* !__POK_MULTIPROCESSING_H__ */
