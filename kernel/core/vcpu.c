#include <core/vcpu.h>
#include <core/sched.h>

struct  vcpu *alloc_vcpu(pok_partition_t * partition, uint8_t vcpu_id)
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

  if ( sched_init_vcpu(v) == 0)
  {
    return 0;
  }

  /*
   * set run state;
   */
  v->runstate = POK_STATE_RUNNABLE;

  if(vcpu_initialize(v) == 0)
  {
    return 0;
  }
  /*
   * add vcpu to list
   */
  return v;
}
