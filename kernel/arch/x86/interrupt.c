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
#include <bsp.h>
#include <core/vcpu.h>
#include <arch/x86/interrupt.h>

void update_tss (interrupt_frame* frame)
{
  uint32_t* esp0 = (&pok_tss) + 1;

  if ((frame->cs & 0xffff) != 0x8)
  {
    *esp0 = (uint32_t)frame + sizeof (interrupt_frame);
  }
}
#ifdef POK_NEEDS_X86_VMM

/*
 * Deal with the interrupt if the interrupt should be handler by guest
 */
void do_IRQ(uint8_t vector)
{
  do_IRQ_guest(vector);
}

/*
 * Decide the interrupt should be send to guest or not
 */
void do_IRQ_guest(uint8_t vector)
{
  uint8_t i,j;
  struct vcpu *v;
  for(i = 0 ; i < POK_CONFIG_NB_PARTITIONS ; i++)
  {
    v = pok_partitions[i].vcpu;
    for (j = 0 ; j< 16; j++)
    {
      if(v->arch.irqdesc[i].vector == vector)
      {
        v->arch.irqdesc[i].pending = TRUE;
	v->pending = TRUE;
	v->arch.irqdesc[i].counter++;
      }
    }
  }
}

/*
 * Check the pending bit when the partition resumes
 */

uint32_t upcall_irq(interrupt_frame* frame)
{
  struct vcpu *v;
  uint8_t i;
  uint32_t _eip;
  uint32_t user_space_handler;
  v = pok_partitions[POK_SCHED_CURRENT_PARTITION].vcpu;
  _eip = frame->eip;		// if no interrupt happened, return the point of normal program;
  user_space_handler = v->arch.handler;
  user_space_handler -= pok_partitions[POK_SCHED_CURRENT_PARTITION].base_addr;
  if(v->pending != 0)
  {
    for(i=0;i<15;i++)
    {
      if(v->arch.irqdesc[i].counter != 0)
      {
        save_interrupt_vcpu(v,frame);
        __upcall_irq(frame, i, (uint32_t) user_space_handler);
        v->arch.irqdesc[i].counter --;
	return user_space_handler;  //if any interrupt occours, return the point of interrupt handler;
      }
    }
  }
  return _eip;
}

/*
 * This function will update the interrupt frame to run the handler of Guest OS
 */
void __upcall_irq(interrupt_frame* frame,uint8_t vector, uint32_t handler)
{
  frame->eax = vector;        //put the irq number to eax
  frame->eip = handler;       //Set the eip as handler
}

/*
 * This do_iret will check the irq_desc,and according to the irq_desc, construct interrupt frame, then iret to execute handler of Guest OS
 */
pok_ret_t do_iret(interrupt_frame *frame)
{
  struct vcpu *v;
  uint8_t i;
  uint32_t user_space_handler;

  v = pok_partitions[POK_SCHED_CURRENT_PARTITION].vcpu;

  user_space_handler = v->arch.handler;
  user_space_handler -= pok_partitions[POK_SCHED_CURRENT_PARTITION].base_addr;
  if(v->pending != 0)
  {
    for(i=0;i<15;i++)
    {
      while(v->arch.irqdesc[i].counter != 0)
      {
        __upcall_irq(frame, i, (uint32_t) user_space_handler);
	v->arch.irqdesc[i].counter--;
	return POK_ERRNO_OK;
      }
    }
    v->pending = 0;
  }
  else if(v->pending == 0)
  {
    restore_interrupt_vcpu(v, frame);
  }

  return POK_ERRNO_OK;
}
#endif /* POK_NEEDS_X86_VMM */
