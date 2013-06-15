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


#ifdef POK_NEEDS_DEBUG

#include <arch.h>
#include <errno.h>
#include <core/debug.h>
#include <core/cons.h>
#include <core/sched.h>
#include <core/thread.h>
#include <core/partition.h>

int debug_strlen (const char* str)
{
  int		i = 0;

  while (*str != '\0')
  {
    str++;
    i++;
  }
  return i;
}

void pok_debug_print_current_state ()
{
   uint32_t i;
   printf ("\nCurrent state\n");
   printf ("-------------\n");
   printf ("Kernel thread        : %d\n", KERNEL_THREAD);
   printf ("Idle thread          : %d\n", IDLE_THREAD);
#ifdef POK_NEEDS_PARTITIONS
   printf ("Current partition    : %d\n", POK_SCHED_CURRENT_PARTITION);
   printf ("Thread index         : %d\n", POK_CURRENT_PARTITION.thread_index);
   printf ("Thread low           : %d\n", POK_CURRENT_PARTITION.thread_index_low);
   printf ("Thread high          : %d\n", POK_CURRENT_PARTITION.thread_index_high);
   printf ("Thread capacity      : %d\n", POK_CURRENT_PARTITION.nthreads);
   printf ("Base addr            : 0x%x\n", POK_CURRENT_PARTITION.base_addr);
   printf ("Base vaddr           : 0x%x\n", POK_CURRENT_PARTITION.base_vaddr);
   printf ("Size                 : %d\n", POK_CURRENT_PARTITION.size);
   printf ("Current thread       : %d\n", POK_CURRENT_PARTITION.current_thread);
   printf ("Prev current thread  : %d\n", POK_CURRENT_PARTITION.prev_thread);
   printf ("Main thread          : %d\n", POK_CURRENT_PARTITION.thread_main);
   printf ("Main thread entry    : 0x%x\n", POK_CURRENT_PARTITION.thread_main_entry);
   printf ("Partition threads sp :");
   for (i = POK_CURRENT_PARTITION.thread_index_low ; i < POK_CURRENT_PARTITION.thread_index_low + POK_CURRENT_PARTITION.thread_index ; i++)
   {
      printf (" 0x%x", pok_threads[i].sp);
   }
   printf ("\n");
   printf ("-------------\n");
#endif
   printf ("Current thread    : %d\n", POK_SCHED_CURRENT_THREAD);
   printf ("Period            : %d\n", POK_CURRENT_THREAD.period);
   printf ("Deadline          : %d\n", POK_CURRENT_THREAD.deadline);
   printf ("Partition         : %d\n", POK_CURRENT_THREAD.partition);
   printf ("sp                : 0x%x\n", POK_CURRENT_THREAD.sp);
   printf ("init_stack_addr   : 0x%x\n", POK_CURRENT_THREAD.init_stack_addr);
   printf ("entry             : 0x%x\n", POK_CURRENT_THREAD.entry);
}

void pok_fatal (const char* message)
{
  pok_write ("FATAL ERROR: \n", 13);
  pok_write (message , debug_strlen(message));

  POK_DEBUG_PRINT_CURRENT_STATE
  pok_arch_idle ();
}

#endif /* POK_CONFIG_NEEDS_DEBUG */
