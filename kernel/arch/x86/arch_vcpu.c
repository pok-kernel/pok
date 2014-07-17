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
  v->arch.interrupt_storge = vcpu_irq_init(struct vcpu *v);
  return POK_ERRNO_OK;
}

pok_ret_t vcpu_irq_init()
{
  memset(v->arch.irqdesc,0,sizeof(irq_desc)*16); 
  irqdesc[0].vector = 32; /* PIC interrupt number */
  return POK_ERRNO_OK;
}
#endif /* POK_NEEDS_X86_VMM */
