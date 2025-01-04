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

/**
 * \file    core/time.c
 * \author  François Goudal
 * \author  Julien Delange
 * \date    2008-2009
 */

#include <bsp.h>
#include <errno.h>
#include <types.h>

#include <core/partition.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/time.h>

/**
 * A global variable that contains the number
 * of elapsed ticks since the beginning
 */
volatile uint64_t pok_tick_counter = 0;

/**
 * \brief Init the timing service.
 *
 * Call BSP-defined function to initialize
 * the timing service.
 */
void pok_time_init(void) {
  pok_tick_counter = 0;
  pok_bsp_time_init();
}
