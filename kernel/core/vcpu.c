#ifdef POK_NEEDS_X86_VMM

#include <core/vcpu.h>

struct  vcpu *alloc_vcpu(partition_t * partition, unsigned int vcpu_id)
{
  struct vcpu *v;
  /*
   * alloc the space of vcpu structrue;
   */
  v = alloc_vcpu_struct();
  
  /*
   * As for now, each vcpu corrsponding one partition, so the vcpu_id as parameter is the index of the partition.
   */
  v->id =vcpu_id;

  v->partition=partition;

  if ( sched_init_vcpu(v,partition) == 0)
  {
	  destroy_vcpu(v);
	  return;
  }

  /*
   * set run state;
   */
  v->runstate = POK_STATE_RUNNABLE;
  if(vcpu_initialize(v) == 0)
  {
    destory_vcpu(v);
    return;
  }
}

#endif
