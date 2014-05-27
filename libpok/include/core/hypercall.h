#ifndef __LIBPOK_HYPERCALL_H__
#define __LIBPOK_HYPERCALL_H__

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
}pok_hypercall_id_t;

typedef struct
{
   uint32_t             nargs;
   uint32_t             arg1;
   uint32_t             arg2;
   uint32_t             arg3;
   uint32_t             arg4;
   uint32_t             arg5;
} pok_hypercall_args_t;


#ifdef POK_ARCH_X86
/*
 * To reduce the number of functions and improve code coverage, we define
 * only one function to perform the hypercall, the other are just maccro
 * This optimization was done only for x86 architecture.
 */
	 pok_ret_t pok_do_hypercall (pok_hypercall_id_t hypercall_id, pok_hypercall_args_t* args);

	 #define pok_hypercall1(sid,arg1) \
					 pok_do_hypercall(sid,&((pok_hypercall_args_t){2,arg1,0,0,0,0}))

	 #define pok_hypercall2(sid,arg1,arg2) \
					 pok_do_hypercall(sid,&((pok_hypercall_args_t){2,arg1,arg2,0,0,0}))

	 #define pok_hypercall3(sid,arg1,arg2,arg3) \
					 pok_do_hypercall(sid,&((pok_hypercall_args_t){2,arg1,arg2,arg3,0,0}))

	 #define pok_hypercall4(sid,arg1,arg2,arg3,arg4) \
					 pok_do_hypercall(sid,&((pok_hypercall_args_t){2,arg1,arg2,arg3,arg4,0}))

	 #define pok_hypercall5(sid,arg1,arg2,arg3,arg4,arg5) \
					 pok_do_hypercall(sid,&((pok_hypercall_args_t){2,arg1,arg2,arg3,arg4,arg5}))
#else

pok_ret_t pok_hypercall1  (pok_hypercall_id_t hypercall_id,
												 uint32_t arg1);

pok_ret_t pok_hypercall2  (pok_hypercall_id_t hypercall_id,
												 uint32_t         arg1,
												 uint32_t         arg2);

pok_ret_t pok_hypercall3 (pok_hypercall_id_t  hypercall_id,
												uint32_t          arg1,
												uint32_t          arg2,
												uint32_t          arg3);

pok_ret_t pok_hypercall4 (pok_hypercall_id_t  hypercall_id,
												uint32_t          arg1,
												uint32_t          arg2,
												uint32_t          arg3,
												uint32_t          arg4);

pok_ret_t pok_hypercall5 (pok_hypercall_id_t  hypercall_id,
												uint32_t arg1,
												uint32_t arg2,
												uint32_t arg3,
												uint32_t arg4,
												uint32_t arg5);
#endif
#endif /* __LIBPOK_HYPERCALL_H__ */
