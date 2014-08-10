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
 *                                      Copyright (c) 2007-2013 POK team 
 *
 * Created by phipse on Wed May 15 13:12:32 2013 
 */

#include <libc/stdio.h>
#include <core/time.h>
#include <core/thread.h>
#include <core/hypercall.h>

#define PIT_IRQ 0

void handle_irq();

uint64_t tick_counter = 0;
void register_handler()
{
  pok_ret_t ret = pok_hypercall2( POK_HYPERCALL_IRQ_REGISTER_VCPU, 0, (uint32_t)&handle_irq);
  if( ret != POK_ERRNO_OK )
    printf( "Couldn't register handler\n");
}


/* this shows, that the handler registration doesn't work propperly.
 * If we don't try to register the handler, the program executes propperly.
 */

int user_hello( void )
{
  printf( "Hello POK world!\n");
  register_handler();

  return 0;
}

void handle_irq()
{
  uint32_t irq=0;
  do{
  asm(
      "add %%eax,%0  \n"	\
      :"=m"(irq)	\
      :
      :"%eax");
  }while(0);
  switch(irq)
  {
case PIT_IRQ:
    tick_counter++;
    printf( "Clock gettick: %u \n",tick_counter);
    pok_hypercall1( POK_HYPERCALL_IRQ_DO_IRET,0);    
    break;

    
default:
    pok_hypercall1( POK_HYPERCALL_IRQ_DO_IRET,0);    
  }
}
