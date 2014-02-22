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
 * Created by julien on Wed Sep 30 17:30:02 2009 
 */

#ifdef POK_NEEDS_INSTRUMENTATION

#include <core/thread.h>
#include <core/partition.h>
#include <core/time.h>
#include <libc.h>

uint8_t pok_instrumentation_task_is_init = 0;
uint8_t pok_instrumentation_partition_is_init = 0;

void pok_instrumentation_task_archi (const uint32_t id)
{
   /*
    * We don't print the first thread, this is the init thread
    */
   if (id == pok_partitions[pok_threads[id].partition].thread_index_low)
   {
      return;
   }

   if (pok_instrumentation_task_is_init == 0)
   {
      printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] </address_spaces>\n");
      printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] <tasks>\n");
   }

   pok_instrumentation_task_is_init = 1;

   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] <task task_type=\"PERIODIC_TYPE\">\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <cpu_name>pokkernel</cpu_name>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <address_space_name>partition%d</address_space_name>\n", pok_threads[id].partition);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <address_space_name>partition%d</address_space_name>\n", pok_threads[id].partition);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <name>task%d</name>\n", id);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <capacity>%d</capacity>\n", pok_threads[id].time_capacity);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <start_time>0</start_time>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <policy>SCHED_FIFO</policy>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <deadline>%d</deadline>\n", pok_threads[id].deadline);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <criticality>0</criticality>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <context_switch_overhead>0</context_switch_overhead>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <blocking_time>0</blocking_time>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <priority>1</priority>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <text_memory_size>0</text_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <stack_memory_size>0</stack_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <period>%d</period>\n", pok_threads[id].period);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]     <jitter>0</jitter>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] </task>\n");
}

void pok_instrumentation_partition_archi (const uint8_t id)
{
   if (pok_instrumentation_partition_is_init == 0)
   {
      printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] <address_spaces>\n");
   }

   pok_instrumentation_partition_is_init = 1;

   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] <address_space>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <name>partition%d</name>\n", id);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <cpu_name>pokkernel</cpu_name>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <text_memory_size> 0</text_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <stack_memory_size> 0</stack_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <data_memory_size> 0</data_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <heap_memory_size> 0</heap_memory_size>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    <scheduler quantum= \"%d\">", pok_partitions[id].period);
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI]    POSIX_1003_HIGHEST_PRIORITY_FIRST_PROTOCOL </scheduler>\n");
   printf ("[INSTRUMENTATION][CHEDDAR][ARCHI] </address_space>\n");
}

void pok_instrumentation_running_task (const uint32_t id)
{
   printf ("[INSTRUMENTATION][CHEDDAR][EVENTS] <running_task>   ");
   printf ("%d", POK_GETTICK() );
   printf ("  task%d", id);
   printf ("</running_task>\n");
}

#endif
