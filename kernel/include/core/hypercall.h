#ifndef __POK_HYPERCALL_H__
#define __POK_HYPERCALL_H__

#include <types.h>
#include <errno.h>

typedef enum
{
   POK_HYPERCALL_CONSWRITE		=10,
   POK_HYPERCALL_INT_NUMBER		=24,
   /* Register an irq handler from a partition */
   POK_HYPERCALL_IRQ_REGISTER_HANDLER		   =  25,
   POK_HYPERCALL_IRQ_UNREGISTER_HANDLER		   =  26,
   /* enable/disable interrupt delivery to a partition */
   POK_HYPERCALL_IRQ_PARTITION_ENABLE		   =  27,
   POK_HYPERCALL_IRQ_PARTITION_DISABLE		   =  28,
   POK_HYPERCALL_IRQ_PARTITION_ACK		   =  29,
} pok_hypercall_id_t;

typedef struct
{
   pok_partition_id_t	partition;
   uint32_t  		thread;
   uint32_t		base_addr;
}pok_hypercall_info_t;

typedef struct
{
	 uint32_t             nargs;
	 uint32_t             arg1;
	 uint32_t             arg2;
	 uint32_t             arg3;
	 uint32_t             arg4;
	 uint32_t             arg5;
} pok_hypercall_args_t;

pok_ret_t pok_core_hypercall(const pok_hypercall_id_t	hypercall_id,
			const pok_hypercall_args_t* args,
			const pok_hypercall_info_t* infos);

pok_ret_t pok_hypercall_init();
#endif /* __POK_HYPERCALL_H__ */
