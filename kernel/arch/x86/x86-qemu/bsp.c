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


#include <errno.h>
#include <arch.h>
#include <arch/x86/interrupt.h> /* meta handler */
#include <core/partition.h>	/* POK_SCHED_CURRENT_PARTITION */

#include "cons.h"
#include "pm.h"
#include "pit.h"
#include "pic.h"

static meta_handler handler_table[16];
static unsigned int partition_irq_enabled[POK_CONFIG_NB_PARTITIONS];

pok_ret_t pok_meta_handler_init( void )
{
  int i, j;
  for( i = 0; i < 16; i++ )
  {
    meta_handler init;
    init.vector = 0xFFF; /* magic number > IDT_SIZE */
    for( j = 0; j < POK_CONFIG_NB_PARTITIONS+1; j++ )
      init.handler[j] = NULL; /* No handlers present */
    handler_table[i] = init; 
  }

  return (POK_ERRNO_OK);
}

pok_ret_t pok_partition_irq_init( void )
{
  int i;
  for( i = 0; i < POK_CONFIG_NB_PARTITIONS; i++ )
  {
    partition_irq_enabled[i] = 0;
  }

  return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_init (void)
{
   pok_cons_init ();
   pok_pm_init ();
   pok_pic_init ();
   pok_meta_handler_init();
   pok_partition_irq_init();

   return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_irq_acknowledge (uint8_t irq)
{
   pok_pic_eoi (irq);

   return (POK_ERRNO_OK);
}

void _C_isr_handler( unsigned vector, interrupt_frame *frame ) 
{
  pok_bsp_irq_acknowledge(vector);

  /* If kernel handler registered */
  if( handler_table[vector].handler[POK_CONFIG_NB_PARTITIONS] != NULL )
    handler_table[vector].handler[POK_CONFIG_NB_PARTITIONS](vector, (void*)frame);

  /* TODO to ensure segregation some code must be written.
   * it has something to do with setting the right CS
   * The CS stored in the interrupt frame should be the right one. 
   */

  /* the following solution kills the cleanup and the update_tss code running
   * after the _C_isr_handler. A solution without Iret is necessary
   * Is the frame set up right?
   */
  if( partition_irq_enabled[POK_SCHED_CURRENT_PARTITION] == 0 )
  {
    if( handler_table[vector].handler[POK_SCHED_CURRENT_PARTITION] != NULL )
    {
      uint32_t *user_space_handler = (uint32_t*) handler_table[vector].handler[POK_SCHED_CURRENT_PARTITION];
      asm volatile(
	/* move interrupt_frame parts to user space to be able to restore the
	 * interrupted context
	 */
	  "movl 56(%1), %%ebx	\t\n" //move user esp address to register

	  "movl 4(%1),	%%eax	\t\n"
	  "movl %%eax,	%%gs	\t\n" // move user's DS to gs register

	  "movl 44(%1), %%eax	\t\n" //move eip to register
	  "movl %%eax,	%%gs:(%%ebx)    \t\n" // move eip to user esp

	  "movl 36(%1), %%eax \t\n"
	  "movl %%eax,	%%gs:-4(%%ebx)  \t\n" //move eax to user esp

	  "movl 32(%1), %%eax \t\n" 
	  "movl %%eax,  %%gs:-8(%%ebx)  \t\n" // move ecx

	  "movl 28(%1), %%eax \t\n"
	  "movl %%eax,	%%gs:-12(%%ebx) \t\n" //move edx

	  "movl 24(%1), %%eax \t\n"
	  "movl %%eax,  %%gs:-16(%%ebx) \t\n" //move ebx

	  "movl 20(%1), %%eax \t\n"
	  "movl %%eax,	%%gs:-20(%%ebx) \t\n" //move __esp

	  "movl 16(%1), %%eax \t\n"
	  "movl %%eax,	%%gs:-24(%%ebx) \t\n" //move ebp

	  "movl 12(%1), %%eax \t\n"
	  "movl %%eax,	%%gs:-28(%%ebx) \t\n" //move esi

	  "movl 8(%1),  %%eax \t\n"
	  "movl %%eax,	%%gs:-32(%%ebx) \t\n" //move edi 

	/* don't move ds and es, as they are already restored before the switch
	 * to user space and are not touched by popa */
	/* prepare the user space stack to look like a propper function call */

	  "movl %%gs:-32(%%ebx), %%eax \t\n"
	  "movl %%eax,	    %%gs:-36(%%ebx) \t\n" //arg2
	  "movl %0,	    %%gs:-40(%%ebx) \t\n" //arg1

	  "movl 44(%1),	    %%eax \t\n"
	  "movl %%eax,	    %%gs:-44(%%ebx) \t\n" // move eip USED AS PADDING!

	  "movl 56(%1),	    %%eax \t\n"
	  "sub	$48,	    %%eax	    \t\n" // update esp in frame
	  "movl %%eax,	    56(%1)	    \t\n"

	/* prepare segment registers for switch to user space */
	
	"movl 4(%1),  %%eax	  \t\n" //move ds to eax
	"movl (%1),   %%ebx	  \t\n"	//move es to eax
	"mov %%ax,    %%ds	  \t\n"
	"mov %%ax,    %%fs	  \t\n"
	"mov %%ax,    %%gs	  \t\n"
	"mov %%bx,    %%es	  \t\n"

	/* prepare stack for iret with user space values saved in interrupt
	 * frame*/
	/* TODO BOGUS if interrupt occurres in kernel space, must be checked! */

	/* delete interrupt frame from stack */
	"movl %1, %%eax \t\n"  // move frame pointer to eax
	"add $44, %%eax \t\n" //  add 44 to the frame pointer
	"movl %%eax, %%esp \t\n" //delete everything on the stack except eip,cs,eflags,esp,ss
	
	/* change eip address to user space handler address */
	"movl %2, (%%esp) \t\n"

	"iret		  \t\n"
      :
      : "r"(vector), "r"(frame), "r"(user_space_handler)
      : "%eax", "%ebx"
      );

      /*
       *  NEVER REACHED!
       */
//      handler_table[vector].handler[POK_SCHED_CURRENT_PARTITION](vector, (void*)frame);
    }
  }
}

pok_ret_t pok_bsp_irq_register_hw (uint8_t   irq,
				   void      (*irq_handler)(unsigned, void*))
{
  if( irq > 15 )
    return (POK_ERRNO_EINVAL);

  pok_pic_unmask (irq);
  
  /* magic number: set in pok_meta_handler_init()
   * meaning: vector not yet used */
  if( handler_table[irq].vector == 0xFFF ) 
    handler_table[irq].vector = irq;

  if( pok_partitions[0].base_addr == 0 || (uint32_t)irq_handler < pok_partitions[0].base_addr)
    /* must be a kernel handler */
    handler_table[irq].handler[POK_CONFIG_NB_PARTITIONS] = irq_handler;
  else
    /* partition handler otherwise */
    handler_table[irq].handler[POK_SCHED_CURRENT_PARTITION] = irq_handler;

  pok_arch_event_register (32 + irq, NULL);

  return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_irq_unregister_hw (uint8_t  irq)
{
  if( irq > 15 )
    return (POK_ERRNO_EINVAL);

  handler_table[irq].handler[POK_SCHED_CURRENT_PARTITION] = NULL;

  /* TODO should check if all handler are detached and then close the irq line.
   */
  
  return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_irq_register (uint8_t   irq,
                                void      (*handler)(void))
{
   pok_pic_unmask (irq);

   pok_arch_event_register (32 + irq, handler);

   return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_irq_partition_enable (uint8_t level)
{
  if( partition_irq_enabled[POK_SCHED_CURRENT_PARTITION] == 0)
    level = 0;
  else
    level = --partition_irq_enabled[POK_SCHED_CURRENT_PARTITION];
  
  level = level; // prevent unused var error

  return (POK_ERRNO_OK);
}

pok_ret_t pok_bsp_irq_partition_disable (uint8_t level)
{
  level = ++partition_irq_enabled[POK_SCHED_CURRENT_PARTITION];
  level = level; // prevent unused var error
  
  return (POK_ERRNO_OK);
}

/**
 * Allocate data. At this time, the pok_pm_sbrk function
 * only increment size each time we allocate memory
 * and was not designed to free previously allocated
 * memory.
 */
void *pok_bsp_mem_alloc (size_t size)
{
   return ((void *)pok_pm_sbrk(size));
}

/**
 * Init time. \a freq is the frequency
 * of the oscillator.
 */
pok_ret_t pok_bsp_time_init ()
{
   return (pok_x86_qemu_timer_init ());
}

