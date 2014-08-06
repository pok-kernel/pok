#include <errno.h>
#include <core/debug.h>
#include <core/partition.h>
#include <core/hypercall.h>

#include "event.h"
#include "gdt.h"

#define PARTITION_ID(cs) (((cs >> 3) - 4 ) / 2)

/*
 * Define the hypercall handler function
 */

INTERRUPT_HANDLER_hypercall(hypercall_gate)
{
   pok_hypercall_info_t hypercall_info;
   pok_ret_t		hypercall_ret;
   pok_hypercall_args_t* hypercall_args;
   pok_hypercall_id_t	hypercall_id;

   hypercall_info.partition = PARTITION_ID (frame->cs);
   hypercall_info.base_addr  = pok_partitions[hypercall_info.partition].base_addr;
   hypercall_info.thread     = POK_SCHED_CURRENT_THREAD;
   hypercall_info.frame	     = frame;

   hypercall_args = (pok_hypercall_args_t * ) (frame->ebx + hypercall_info.base_addr);

   hypercall_id = (pok_hypercall_id_t) frame->eax;

   if (POK_CHECK_PTR_IN_PARTITION(hypercall_info.partition, hypercall_args) == 0)
   {
      hypercall_ret = POK_ERRNO_EINVAL;
   }
   else
   {
	// perform the hypercall;
      hypercall_ret = pok_core_hypercall ( hypercall_id, hypercall_args, & hypercall_info);
   }
   /*
    * put the return value in eax register
    */
   asm ( "movl %0, %%eax \n"
    :
    : "m" (hypercall_ret));
}

/*
 * Init Hypercall system
 */

pok_ret_t pok_hypercall_init()
{
   pok_idt_set_gate (POK_HYPERCALL_INT_NUMBER,
		   GDT_CORE_CODE_SEGMENT << 3,
		   (uint32_t) hypercall_gate,
		   IDTE_INTERRUPT,
		   3);
   return (POK_ERRNO_OK);
}
