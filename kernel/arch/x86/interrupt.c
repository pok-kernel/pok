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


#include <arch/x86/interrupt.h>
#include <core/vcpu.h>

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

void upcall_irq()
{
  struct vcpu *v;
  v = pok_partitions[POK_SCHED_CURRENT_PARTITION].vcpu;
  if(v->pending != 0)
  {
    __upcall_irq(v->arch.irqdesc);
  }
}
void __upcall_irq(struct irq_desc *irqdescs)
{
  uint8_t i;
  for(i=0; i<16; i++)
  {
    while(irqdescs[i].counter)
    {
//      handler_irq(irqdescs[i].vector);
      irqdescs[i].counter--;
    }
  }
}


#endif /* POK_NEEDS_X86_VMM */
