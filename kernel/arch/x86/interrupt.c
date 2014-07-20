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

void do_IRQ(uint8_t vector)
{
  do_IRQ_guest(vector);
}

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
      }
    }
  }
}

#endif /* POK_NEEDS_X86_VMM */
