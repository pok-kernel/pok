#ifdef POK_NEEDS_X86_VMM
#ifndef __POK_VCPU_H__
#define __POL_VCPU_H__

#include <types.h>
#include <errno.h>


typedef struct vcpu
{
   /*
    * This number is the id of a vCPU.
    */
   unsigned  int id;
   /*
    * This is a list of vcpu, to organize vcpus in logic.
    */
   struct vcpu *prev, *next;
   /*
    * This is a point to the partition that the vcpu bond to.
    */
   pok_partition_t * partition;

   /*
    * This runstate indicate the state of vcpu.
    */
   pok_state_t runstate;

   /*
    * This is the schedule information.
    * In this struct, there are some time information for scheduler
    */
   vcpu_sched_info_t sched_info;

   /*
    * This struct contains some arch-dependent attributes and operations.
    */
   vcpu_arch_t arch;

   /*
    * This bit will be set ture if there is a interrupt pending.
    */
   bool_t pending;

}vcpu_t;

struct vcpu *alloc_vcpu( partition_t * partition, unsigned int vcpu_id);
struct vcpu *alloc_vcpu_struct(void);
void free_vcpu_struct(struct vcpu *);

#endif
#endif
