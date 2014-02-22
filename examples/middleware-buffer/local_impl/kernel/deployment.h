/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#ifndef __POK_KERNEL_GENERATED_DEPLOYMENT_H_
#define __POK_KERNEL_GENERATED_DEPLOYMENT_H_ 

#define POK_NEEDS_TIME       1
#define POK_NEEDS_THREADS    1
#define POK_NEEDS_PARTITIONS 1
#define POK_NEEDS_SCHED      1
#define POK_NEEDS_DEBUG      1
#define POK_NEEDS_LOCKOBJECTS 1

#define POK_CONFIG_NB_THREADS       3
#define POK_CONFIG_NB_PARTITIONS    1
#define POK_CONFIG_PARTITIONS_NLOCKOBJECTS  {1,0}

#define POK_CONFIG_NB_LOCKOBJECTS       1

#define POK_CONFIG_PARTITIONS_SIZE  {16 * 1024};
#define POK_CONFIG_PROGRAM_NAME {"pr1/pr1.elf"};
#define POK_CONFIG_PARTITIONS_NTHREADS  {3}
#define POK_CONFIG_SCHEDULING_SLOTS {5000}
#define POK_CONFIG_SCHEDULING_MAJOR_FRAME 2000
#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION {0}
#define POK_CONFIG_SCHEDULING_NBSLOTS 1

#define POK_CONFIG_NB_PORTS         0

#define POK_CONFIG_NB_ALLPORTS  0

#define POK_KERNEL_PART 1

#define POK_KERNELLAND

typedef enum
{
   pok_part_identifier_pr1 = 0,
   pok_part_identifier_pr2 = 1
}pok_part_identifiers_t;

typedef enum
{
   pok_part_id_pr1 = 0,
   pok_part_id_pr2 = 1
}pok_part_id_t;


typedef enum
{
   dataout = 0
}pok_parts_ports_pr1_identifiers_t;

typedef enum
{
   datain = 0
}pok_parts_ports_pr2_identifiers_t;

typedef enum
{
   ports_global_pr1_dataout = 0,
   ports_global_pr2_datain  = 1
}pok_parts_ports_global_identifiers_t;

typedef enum
{
   port_all_pr1_dataout = 0,
   port_all_pr2_datain  = 1
}pok_parts_ports_all_identitiers_t;

typedef enum
{
   pok_nodes_node1 = 0
}pok_node_id_t;

#define MY_NODE pok_nodes_node1

#endif
