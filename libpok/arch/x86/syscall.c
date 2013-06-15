/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#include <core/syscall.h>
#include <types.h>

pok_ret_t pok_do_syscall (pok_syscall_id_t syscall_id, pok_syscall_args_t* args)
{
   pok_ret_t   ret;
   uint32_t    args_addr;
   uint32_t    id;

   args_addr = (uint32_t) args;
   id        = (uint32_t) syscall_id;

   asm volatile ( "movl %1,%%eax \n\t"
                  "movl %2,%%ebx \n\t"
                  "int  $42 \n\t"
                  "movl %%eax, %0"
                  :"=g"(ret)
                  :"g"(id), "g"(args_addr)
                  : "%eax" , "%ebx"
                  );
   return ret;
}

