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

#ifndef __POK_KERNEL_GENERATED_DEPLOYMENT_H_
#define __POK_KERNEL_GENERATED_DEPLOYMENT_H_

#define POK_NEEDS_TIME 1
#define POK_NEEDS_MIDDLEWARE 1
#define POK_NEEDS_PORTS_SAMPLING 1

#define POK_CONFIG_NB_THREADS 4
#define POK_CONFIG_NB_PARTITIONS 2

#define POK_CONFIG_PARTITIONS_SIZE {120 * 1024, 120 * 1024};
#define POK_CONFIG_PARTITIONS_NTHREADS                                         \
  { 2, 2 }
#define POK_CONFIG_SCHEDULING_SLOTS                                            \
  { 2000000000, 2000000000 }
#define POK_CONFIG_SCHEDULING_MAJOR_FRAME 4000000000
#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION                                 \
  { 0, 1 }
#define POK_CONFIG_SCHEDULING_NBSLOTS 2

#define POK_CONFIG_NB_PORTS 2

#define POK_CONFIG_NB_ALLPORTS 2

#define POK_NEEDS_DEBUG

typedef enum { dataout = 0, datain = 1 } pok_ports_identitiers_t;

#endif
