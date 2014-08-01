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

static unsigned counter = 0;

void time_handler( unsigned vector, void* frame )
{
  (void) frame;
  (void)vector;
  counter++;
}


void register_handler()
{
  pok_ret_t ret = pok_hypercall2( POK_HYPERCALL_IRQ_REGISTER_HANDLER, 0, (uint32_t)&time_handler );
  if( ret != POK_ERRNO_OK )
    printf( "Couldn't register handler\n");
}


/* this shows, that the handler registration doesn't work propperly.
 * If we don't try to register the handler, the program executes propperly.
 */

static int flag = 0;
int user_hello( void )
{
  printf( "Hello POK world!\n");
//  if (flag < 1)
//  {
//    register_handler();
//    flag++;
//  }
  pok_time_t tmp = 0;
  uint8_t _level = 0;

  pok_hypercall2(POK_HYPERCALL_IRQ_REGISTER_VCPU , PIT_IRQ);

  printf( "Forever spin\n");
  for( ;; )
  {
//    pok_hypercall2(POK_HYPERCALL_GETTICK, (uint32_t)(&tmp),0);
//    if( tmp >= 2000 && tmp <= 3000 )
//    {
//      pok_hypercall1( POK_HYPERCALL_IRQ_PARTITION_DISABLE, _level );
//      printf( "Counter: %u\n", counter );
//    }
//    if( tmp >= 6000 && tmp <= 8000 )
//    {
//      pok_hypercall1( POK_HYPERCALL_IRQ_PARTITION_ENABLE, _level );
//      printf( "Counter: %u\n", counter );
//    }
//
//    printf( "Clock gettick: %u \n", tmp );
  }

  return 0;
}
