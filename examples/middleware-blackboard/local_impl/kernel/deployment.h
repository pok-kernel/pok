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
#define POK_NEEDS_CONSOLE 1
#define POK_NEEDS_LOCKOBJECTS 1

#define POK_CONFIG_NB_THREADS 3
#define POK_CONFIG_NB_PARTITIONS 1

#define POK_CONFIG_PARTITIONS_NLOCKOBJECTS                                     \
  { 1, 0 }

#define POK_CONFIG_PARTITIONS_SIZE {100 * 1024};
#define POK_CONFIG_PROGRAM_NAME {"pr1/pr1.elf"};
#define POK_CONFIG_SCHEDULING_SLOTS                                            \
  { 2000000000 }
#define POK_CONFIG_SCHEDULING_MAJOR_FRAME 2000000000
#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION                                 \
  { 0 }
#define POK_CONFIG_SCHEDULING_NBSLOTS 1

#define POK_CONFIG_NB_LOCKOBJECTS 1

#define POK_CONFIG_PARTITIONS_NTHREADS                                         \
  { 3 }

#define POK_CONFIG_NB_PORTS 0

#define POK_CONFIG_NB_ALLPORTS 0

typedef enum {
  pok_part_identifier_pr1 = 0,
  pok_part_identifier_pr2 = 1
} pok_part_identifiers_t;

typedef enum { pok_part_id_pr1 = 0, pok_part_id_pr2 = 1 } pok_part_id_t;

typedef enum { pok_nodes_node1 = 0 } pok_node_id_t;

#endif
