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
  vcpu_irq_context_init(v);
  return POK_ERRNO_OK;
}

pok_ret_t vcpu_irq_init(struct vcpu *v)
{
  memset(v->arch.irqdesc,0,sizeof(struct irq_desc)*16); 
//  irqdesc[0].vector = 32; /* PIC interrupt number */
  return POK_ERRNO_OK;
}

pok_ret_t vcpu_irq_context_init(struct vcpu *v)
{
  memset(&(v->arch.vcpu_context), 0, sizeof (struct vcpu_context));
  v->arch.vcpu_context.lastframe = -1;
  return POK_ERRNO_OK;
}
void save_interrupt_vcpu(struct vcpu *v, interrupt_frame * frame)
{
  
  if(v->arch.vcpu_context.lastframe!= 15)
  {
    int8_t lastframe;
    v->arch.vcpu_context.lastframe++;
    lastframe = v->arch.vcpu_context.lastframe;
    v->arch.vcpu_context.frame[lastframe].es     = frame->es    ;
    v->arch.vcpu_context.frame[lastframe].ds     = frame->ds    ;
    v->arch.vcpu_context.frame[lastframe].edi    = frame->edi   ;
    v->arch.vcpu_context.frame[lastframe].esi    = frame->esi   ;
    v->arch.vcpu_context.frame[lastframe].ebp    = frame->ebp   ;
    v->arch.vcpu_context.frame[lastframe].__esp   = frame->__esp  ;
    v->arch.vcpu_context.frame[lastframe].ebx    = frame->ebx   ;
    v->arch.vcpu_context.frame[lastframe].edx    = frame->edx   ;
    v->arch.vcpu_context.frame[lastframe].ecx    = frame->ecx   ;
    v->arch.vcpu_context.frame[lastframe].eax    = frame->eax   ;
    v->arch.vcpu_context.frame[lastframe].error  = frame->error ;
    v->arch.vcpu_context.frame[lastframe].eip    = frame->eip   ;
    v->arch.vcpu_context.frame[lastframe].eflags = frame->eflags;
    v->arch.vcpu_context.frame[lastframe].esp    = frame->esp   ;
    v->arch.vcpu_context.frame[lastframe].ss     = frame->ss    ;
  }
}

void restore_interrupt_vcpu(struct vcpu *v, interrupt_frame * frame)
{
  if(v->arch.vcpu_context.lastframe != -1)
  {
    int8_t lastframe;
    lastframe = v->arch.vcpu_context.lastframe;
    frame->es     =v->arch.vcpu_context.frame[lastframe].es     ;
    frame->ds     =v->arch.vcpu_context.frame[lastframe].ds     ;
    frame->edi    =v->arch.vcpu_context.frame[lastframe].edi    ;
    frame->esi    =v->arch.vcpu_context.frame[lastframe].esi    ;
    frame->ebp    =v->arch.vcpu_context.frame[lastframe].ebp    ;
    frame->__esp  =v->arch.vcpu_context.frame[lastframe].__esp  ;
    frame->ebx    =v->arch.vcpu_context.frame[lastframe].ebx    ;
    frame->edx    =v->arch.vcpu_context.frame[lastframe].edx    ;
    frame->ecx    =v->arch.vcpu_context.frame[lastframe].ecx    ;
    frame->eax    =v->arch.vcpu_context.frame[lastframe].eax    ;
    frame->error  =v->arch.vcpu_context.frame[lastframe].error  ;
    frame->eip    =v->arch.vcpu_context.frame[lastframe].eip    ;
    frame->eflags =v->arch.vcpu_context.frame[lastframe].eflags ;
    frame->esp    =v->arch.vcpu_context.frame[lastframe].esp    ;
    frame->ss     =v->arch.vcpu_context.frame[lastframe].ss     ;
    v->arch.vcpu_context.lastframe--;
  }
}
#endif /* POK_NEEDS_X86_VMM */
