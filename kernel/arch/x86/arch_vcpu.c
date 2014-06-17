#ifdef POK_NEEDS_X86_VMM

#include <core/vcpu.h>
#include <libc.h>

/*
 * Note: The POK did not support space free for now.
 */
struct vcpu * alloc_vcpu_struct(void)
{
  v = ((vcpu_t *)pok_bsp_mem_alloc(sizeof(*v)));
}

#endif
