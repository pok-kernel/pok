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

int vcpu_initialize(struct vcpu *v)
{
  v->arch.ctxt_switch_from = sched_ctxt_switch_from_vcpu;
  v->arch.ctxt_switch_to   = sched_ctxt_switch_to_vcpu;
  v->arch.schedule_tail    = sched_tail_vcpu;
  return 1;
}

#endif /* POK_NEEDS_X86_VMM */
