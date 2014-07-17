
#include <bsp.h>
#include <types.h>
#include <libc.h>
#include <arch/x86/ioports.h>
#include <arch/x86/pci.h>

#include <errno.h>
#include <core/debug.h>
#include <core/hypercall.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/lockobj.h>
#include <core/time.h>
#include <core/error.h>

#include <middleware/port.h>



pok_ret_t pok_core_hypercall (const pok_hypercall_id_t       hypercall_id,
                            const pok_hypercall_args_t*    args,
                            const pok_hypercall_info_t*    infos)
{
   switch (hypercall_id)
   {
#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG)
      case POK_HYPERCALL_CONSWRITE:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         if (pok_cons_write ((const char*)args->arg1 + infos->base_addr, args->arg2))
         {
            return POK_ERRNO_OK;
         }
         else
         {
            return POK_ERRNO_EINVAL;
         }
         break;
#endif

#ifdef POK_NEEDS_X86_VMM
      /**
       * This shall register an irq handler with the meta_handler for the
       * requested interrupt number.
       * arg1: vector nubmer, 
       * arg2: handler/callback function
       */
     case POK_HYPERCALL_IRQ_REGISTER_HANDLER:
       POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg2 + infos->base_addr)
       /* arg1 is just a number, arg2 is a pointer to a function in the
	* partition, therefore the partitions base_addr must be added to become
	* a valid pointer for the kernel.
	*/
       return pok_bsp_irq_register_hw (args->arg1, 
		 (void(*)(unsigned, void*)) ((args->arg2 + infos->base_addr)) );
       break;
     
     case POK_HYPERCALL_IRQ_UNREGISTER_HANDLER:
       return pok_bsp_irq_unregister_hw (args->arg1);
       break;
   /* enable/disable interrupt delivery to a partition */
    case POK_HYPERCALL_IRQ_PARTITION_ENABLE:
       return pok_bsp_irq_partition_enable (args->arg1);
       break;
    case POK_HYPERCALL_IRQ_PARTITION_DISABLE:
       return pok_bsp_irq_partition_disable (args->arg1);
       break;
    case POK_HYPERCALL_IRQ_PARTITION_ACK:
       return pok_bsp_irq_partition_ack(args->arg1);
       break;

   /* register interrupt delivery to vcpu */
   case POK_HYPERCALL_IRQ_REGISTER_VCPU:
       return pok_bsp_irq_register_vcpu(args->arg1);
       break;
       
#endif /* POK_NEEDS_X86_VMM */

#if 1//defined POK_NEEDS_GETTICK
      case POK_SYSCALL_GETTICK:
         POK_CHECK_PTR_OR_RETURN(infos->partition, args->arg1 + infos->base_addr)
         return pok_gettick_by_pointer ((uint64_t*) (args->arg1 + infos->base_addr));
         break;
#endif
	 
    default:
#ifdef POK_NEEDS_ERROR_HANDLING
         pok_error_declare( POK_ERROR_KIND_ILLEGAL_REQUEST);
	 pok_sched_activate_error_thread ();
#else
       #ifdef POK_NEEDS_DEBUG
	 printf("Tried to use hypercall %d\n", hypercall_id);
       #endif
	 POK_FATAL ("Unknown hypercall");
#endif
	 break;
   }
   return POK_ERRNO_EINVAL;
}
