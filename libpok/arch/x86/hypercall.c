
#include <core/hypercall.h>
#include <types.h>

pok_ret_t pok_do_hypercall (pok_hypercall_id_t hypercall_id, pok_hypercall_args_t* args)
{
   pok_ret_t   ret;
   uint32_t    args_addr;
   uint32_t    id;

   args_addr = (uint32_t) args;
   id        = (uint32_t) hypercall_id;

   asm volatile ( "movl %1,%%eax \n\t"
                  "movl %2,%%ebx \n\t"
                  "int  $45 \n\t"
                  "movl %%eax, %0"
                  :"=g"(ret)
                  :"g"(id), "g"(args_addr)
                  : "%eax" , "%ebx"
                  );
   return ret;
}

