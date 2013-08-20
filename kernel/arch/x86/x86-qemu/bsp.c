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

pok_ret_t pok_bsp_init (void)
{
   pok_cons_init ();
   pok_pm_init ();
   pok_pic_init ();
   pok_meta_handler_init();

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
   */

  if( handler_table[vector].handler[POK_SCHED_CURRENT_PARTITION] != NULL )
    handler_table[vector].handler[POK_SCHED_CURRENT_PARTITION](vector, (void*)frame);

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

