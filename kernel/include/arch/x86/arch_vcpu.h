#ifdef POK_NEEDS_X86_VMM
#ifndef __POK_ARCH_VCPU_H__
#define __POK_ARCH_VCPU_H__

#include <types.h>
#include <../arch/x86/thread.h>
#include "interrupt.h"

typedef struct irq_desc
{
   interrupt_frame frame;
   unsigned vector;
   bool_t pending;
}irq_desc_t;

typedef struct trap_info
{
  uint8_t vector; /* exception vector */
  uint8_t flags; /* 0-3 privilege level */
  uint16_t cs;	 /* code selector */
}trap_info_t;

typedef struct vcpu_context
{
  context_t user_regs;
//  trap_info_t trap_ctxt[256];
  unsigned long ctrlreg[8];
//  unsigned long kernel_ss, kernel_sp;
}vcpu_context_t;

typedef struct vcpu vcpu_t;

/*
 * This structure is arch-dependent structure for vcpu.
 */
typedef struct arch_vcpu
{
   /*
    * This struct contains the context of a vcpu, this part is arch-dependent.
    */
   vcpu_context_t  vcpu_context;
   /*
    * This point of function will be bond to arch-depentent function when initialize vcpu struct.
    */
   void (*ctxt_switch_from) (struct vcpu *);
   void (*ctxt_switch_to) (struct vcpu *);
   void (*schedule_tail) (struct vcpu *);

   /*
    * This is a interrupt frame, the interrupt information will be store in this struct;
    */
   struct irq_desc irqdesc[16];

}arch_vcpu_t;

pok_ret_t vcpu_initialize(struct vcpu *);
pok_ret_t vcpu_irq_init();


#endif /* __POK_ARCHVCPU_H__ */
#endif /* POK_NEEDS_X86_VMM */
