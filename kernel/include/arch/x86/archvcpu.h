#ifdef POK_NEEDS_X86_VMM
#ifndef __POK_ARCHVCPU_H__
#define __POK_ARCHVCPU_H__

#include <types.h>
#include "thread.h"
#include "interrupt.h"

typedef struct
{
   interrupt_frame frame;
   unsigned vector;
}interrupt_storage;

/*
 * This structure is arch-dependent structure for vcpu.
 */
typedef struct arch_vcpu
{
   /*
    * This struct contains the context of a vcpu, this part is arch-dependent.
    */
   context_t  guest_context;
   /*
    * This point of function will be bond to arch-depentent function when initialize vcpu struct.
    */
   void (ctxt_switch_from) (struct vcpu *);
   void (ctxt_switch_to) (struct vcpu *);
   void (schedule_tail) (struct vcpu *);

   /*
    * This is a interrupt frame, the interrupt information will be store in this struct;
    */

}arch_vcpu_t;



#endif
#endif
