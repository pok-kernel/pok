#ifdef POK_NEEDS_X86_VMM

#include <bsp.h>
#include <core/vcpu.h>
#include <libc.h>
#include <core/sched.h>

/*
 * Note: The POK did not support space free for now.
 */
struct vcpu * alloc_vcpu_struct(void)
{
  struct vcpu *v;
  v = ((vcpu_t *)pok_bsp_mem_alloc(sizeof(*v)));
  return v;
}

pok_ret_t vcpu_initialize(struct vcpu *v)
{
  v->arch.ctxt_switch_from = sched_ctxt_switch_from_vcpu;
  v->arch.ctxt_switch_to   = sched_ctxt_switch_to_vcpu;
  v->arch.schedule_tail    = sched_tail_vcpu;
  vcpu_irq_init(v);
  return POK_ERRNO_OK;
}

pok_ret_t vcpu_irq_init(struct vcpu *v)
{
  memset(v->arch.irqdesc,0,sizeof(struct irq_desc)*16); 
//  irqdesc[0].vector = 32; /* PIC interrupt number */
  return POK_ERRNO_OK;
}
void save_interrupt_vcpu(struct vcpu *v, interrupt_frame * frame)
{
  v->arch.vcpu_context.frame.es     = frame->es    ;
  v->arch.vcpu_context.frame.ds     = frame->ds    ;
  v->arch.vcpu_context.frame.edi    = frame->edi   ;
  v->arch.vcpu_context.frame.esi    = frame->esi   ;
  v->arch.vcpu_context.frame.ebp    = frame->ebp   ;
  v->arch.vcpu_context.frame.__esp   = frame->__esp  ;
  v->arch.vcpu_context.frame.ebx    = frame->ebx   ;
  v->arch.vcpu_context.frame.edx    = frame->edx   ;
  v->arch.vcpu_context.frame.ecx    = frame->ecx   ;
  v->arch.vcpu_context.frame.eax    = frame->eax   ;
  v->arch.vcpu_context.frame.error  = frame->error ;
  v->arch.vcpu_context.frame.eip    = frame->eip   ;
  v->arch.vcpu_context.frame.eflags = frame->eflags;
  v->arch.vcpu_context.frame.esp    = frame->esp   ;
  v->arch.vcpu_context.frame.ss     = frame->ss    ;
}

void restore_interrupt_vcpu(struct vcpu *v, interrupt_frame * frame)
{
   frame->es    =v->arch.vcpu_context.frame.es     ;
   frame->ds    =v->arch.vcpu_context.frame.ds     ;
   frame->edi   =v->arch.vcpu_context.frame.edi    ;
   frame->esi   =v->arch.vcpu_context.frame.esi    ;
   frame->ebp   =v->arch.vcpu_context.frame.ebp    ;
   frame->__esp  =v->arch.vcpu_context.frame.__esp   ;
   frame->ebx   =v->arch.vcpu_context.frame.ebx    ;
   frame->edx   =v->arch.vcpu_context.frame.edx    ;
   frame->ecx   =v->arch.vcpu_context.frame.ecx    ;
   frame->eax   =v->arch.vcpu_context.frame.eax    ;
   frame->error =v->arch.vcpu_context.frame.error  ;
   frame->eip   =v->arch.vcpu_context.frame.eip    ;
   frame->eflags=v->arch.vcpu_context.frame.eflags ;
   frame->esp   =v->arch.vcpu_context.frame.esp    ;
   frame->ss    =v->arch.vcpu_context.frame.ss     ;
}
#endif /* POK_NEEDS_X86_VMM */
