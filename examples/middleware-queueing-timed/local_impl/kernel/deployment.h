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
#define POK_NEEDS_MIDDLEWARE 1
#define POK_NEEDS_DEBUG
#define POK_NEEDS_PORTS_QUEUEING 1

#define POK_CONFIG_NB_THREADS       4
#define POK_CONFIG_NB_PARTITIONS    2

#define POK_CONFIG_PARTITIONS_SIZE  {12*1024,12*1024}
#define POK_CONFIG_PARTITIONS_NTHREADS  {2,2}
#define POK_CONFIG_PARTITIONS_TIMESLICE  {2000,2000}

#define POK_CONFIG_SCHEDULING_SLOTS {3000, 1500}
#define POK_CONFIG_SCHEDULING_MAJOR_FRAME 4000
#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION {0,1}
#define POK_CONFIG_SCHEDULING_NBSLOTS 2

#define POK_CONFIG_NB_PORTS         2

#define POK_CONFIG_NB_ALLPORTS  2


typedef enum
{
   dataout = 0,
   datain = 1
}pok_ports_identifiers_t;

#endif
