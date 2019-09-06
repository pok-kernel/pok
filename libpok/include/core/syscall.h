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

#ifndef __LIBPOK_SYSCALL_H__
#define __LIBPOK_SYSCALL_H__

#include <types.h>
#include <errno.h>

typedef enum
{
	 POK_SYSCALL_CONSWRITE                           =  10,
	 POK_SYSCALL_GETCHAR                             =  11,
	 POK_SYSCALL_GETTICK                             =  20,
	 POK_SYSCALL_INT_NUMBER                          =  42,
	 POK_SYSCALL_THREAD_CREATE                       =  50,
	 POK_SYSCALL_THREAD_SLEEP_UNTIL                  =  51,
	 POK_SYSCALL_THREAD_SLEEP                        =  52,
	 POK_SYSCALL_THREAD_SUSPEND                      =  53,
	 POK_SYSCALL_THREAD_RESTART                      =  54,
	 POK_SYSCALL_THREAD_STOP                         =  55,
	 POK_SYSCALL_THREAD_PERIOD                       =  56,
	 POK_SYSCALL_THREAD_STOPSELF                     =  57,
	 POK_SYSCALL_THREAD_ID                           =  58,
	 POK_SYSCALL_THREAD_STATUS                       =  59,
	 POK_SYSCALL_THREAD_SET_PRIORITY								 =  60,
	 POK_SYSCALL_THREAD_RESUME                       =  61,
	 POK_SYSCALL_THREAD_SUSPEND_TARGET               =  62,
   POK_SYSCALL_THREAD_DEADLINE                     =  63,
   POK_SYSCALL_THREAD_STATE                        =  64,
   POK_SYSCALL_THREAD_DELAYED_START		   =  65,
#ifdef POK_NEEDS_PORTS_SAMPLING
	 POK_SYSCALL_MIDDLEWARE_SAMPLING_ID              = 101,
	 POK_SYSCALL_MIDDLEWARE_SAMPLING_READ            = 102,
	 POK_SYSCALL_MIDDLEWARE_SAMPLING_STATUS          = 103,
	 POK_SYSCALL_MIDDLEWARE_SAMPLING_WRITE           = 104,
	 POK_SYSCALL_MIDDLEWARE_SAMPLING_CREATE          = 105,
#endif
#ifdef POK_NEEDS_PORTS_QUEUEING
	 POK_SYSCALL_MIDDLEWARE_QUEUEING_CREATE          = 110,
	 POK_SYSCALL_MIDDLEWARE_QUEUEING_SEND            = 111,
	 POK_SYSCALL_MIDDLEWARE_QUEUEING_RECEIVE         = 112,
	 POK_SYSCALL_MIDDLEWARE_QUEUEING_ID              = 113,
	 POK_SYSCALL_MIDDLEWARE_QUEUEING_STATUS          = 114,
#endif
#ifdef POK_NEEDS_MAC_ADDR
	 POK_SYSCALL_MIDDLEWARE_NODE_MAC_ADDR            = 120,
#endif
#ifdef POK_NEEDS_PORTS_VIRTUAL
	 POK_SYSCALL_MIDDLEWARE_VIRTUAL_CREATE           = 150,
	 POK_SYSCALL_MIDDLEWARE_VIRTUAL_NB_DESTINATIONS  = 151,
	 POK_SYSCALL_MIDDLEWARE_VIRTUAL_DESTINATION      = 152,
	 POK_SYSCALL_MIDDLEWARE_VIRTUAL_GET_GLOBAL       = 153,
	 POK_SYSCALL_MIDDLEWARE_VIRTUAL_NODE             = 154,
#endif
#if defined (POK_NEEDS_LOCKOBJECTS) || defined (POK_NEEDS_MUTEXES) || defined (POK_NEEDS_SEMAPHORES) || defined (POK_NEEDS_EVENTS) || defined (POK_NEEDS_BUFFERS) || defined (POK_NEEDS_BLACKBOARDS)
	 POK_SYSCALL_LOCKOBJ_CREATE                      = 201,
	 POK_SYSCALL_LOCKOBJ_OPERATION                   = 202,
#endif
#ifdef POK_NEEDS_ERROR_HANDLING
	 POK_SYSCALL_ERROR_HANDLER_CREATE                = 301,
	 POK_SYSCALL_ERROR_HANDLER_SET_READY             = 302,
	 POK_SYSCALL_ERROR_RAISE_APPLICATION_ERROR       = 303,
	 POK_SYSCALL_ERROR_GET                           = 304,
#endif
#ifdef POK_NEEDS_PARTITIONS
	 POK_SYSCALL_PARTITION_SET_MODE                  = 404,
	 POK_SYSCALL_PARTITION_GET_ID										 = 405,
	 POK_SYSCALL_PARTITION_GET_PERIOD                = 406,
	 POK_SYSCALL_PARTITION_GET_DURATION              = 407,
	 POK_SYSCALL_PARTITION_GET_LOCK_LEVEL            = 408,
	 POK_SYSCALL_PARTITION_GET_OPERATING_MODE        = 409,
	 POK_SYSCALL_PARTITION_GET_START_CONDITION       = 410,
#endif
#ifdef POK_NEEDS_IO
	 POK_SYSCALL_INB                                 = 501,
	 POK_SYSCALL_OUTB                                = 502,
#endif
#ifdef POK_NEEDS_PCI
	 POK_SYSCALL_PCI_REGISTER                        = 601,
#endif

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
	 POK_SYSCALL_RTL8929_READ                        = 701,
	 POK_SYSCALL_RTL8929_POLL_AND_READ               = 702,
	 POK_SYSCALL_RTL8929_WRITE                       = 703,
	 POK_SYSCALL_RTL8929_POLL                        = 704,
	 POK_SYSCALL_RTL8929_CLEAR                       = 705,
	 POK_SYSCALL_RTL8929_INIT                        = 706
#endif
	 
} pok_syscall_id_t;

typedef struct
{
	 uint32_t             nargs;
	 uint32_t             arg1;
	 uint32_t             arg2;
	 uint32_t             arg3;
	 uint32_t             arg4;
	 uint32_t             arg5;
} pok_syscall_args_t;


#ifdef POK_ARCH_X86
/*
 * To reduce the number of functions and improve code coverage, we define
 * only one function to perform the syscall, the other are just maccro
 * This optimization was done only for x86 architecture.
 */
	 pok_ret_t pok_do_syscall (pok_syscall_id_t syscall_id, pok_syscall_args_t* args);

	 #define pok_syscall1(sid,arg1) \
					 pok_do_syscall(sid,&((pok_syscall_args_t){2,arg1,0,0,0,0}))

	 #define pok_syscall2(sid,arg1,arg2) \
					 pok_do_syscall(sid,&((pok_syscall_args_t){2,arg1,arg2,0,0,0}))

	 #define pok_syscall3(sid,arg1,arg2,arg3) \
					 pok_do_syscall(sid,&((pok_syscall_args_t){2,arg1,arg2,arg3,0,0}))

	 #define pok_syscall4(sid,arg1,arg2,arg3,arg4) \
					 pok_do_syscall(sid,&((pok_syscall_args_t){2,arg1,arg2,arg3,arg4,0}))

	 #define pok_syscall5(sid,arg1,arg2,arg3,arg4,arg5) \
					 pok_do_syscall(sid,&((pok_syscall_args_t){2,arg1,arg2,arg3,arg4,arg5}))
#else

pok_ret_t pok_syscall1  (pok_syscall_id_t syscall_id,
												 uint32_t arg1);

pok_ret_t pok_syscall2  (pok_syscall_id_t syscall_id,
												 uint32_t         arg1,
												 uint32_t         arg2);

pok_ret_t pok_syscall3 (pok_syscall_id_t  syscall_id,
												uint32_t          arg1,
												uint32_t          arg2,
												uint32_t          arg3);

pok_ret_t pok_syscall4 (pok_syscall_id_t  syscall_id,
												uint32_t          arg1,
												uint32_t          arg2,
												uint32_t          arg3,
												uint32_t          arg4);

pok_ret_t pok_syscall5 (pok_syscall_id_t  syscall_id,
												uint32_t arg1,
												uint32_t arg2,
												uint32_t arg3,
												uint32_t arg4,
												uint32_t arg5);
#endif
#endif /* __LIBPOK_SYSCALL_H__ */
