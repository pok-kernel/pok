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
#include <core/syscall.h>

#include "virtualizationlayerbsp.h"
#include "virtualizationlayercpu.h"


/**
 * @brief Forwards the interrupt to the partition and returns to the point of
 *	  interruption.
 *
 * Recovers the interrupted context and the vector number from the stack, calls
 * the RTEMS interrupt handler, restores the context afterwards and returns to
 * the interrupted instruction.
 */
void interrupt_middleman( unsigned vector, void* frame )
{
  // search for the magic value 0xffeeddcc on the stack to determin the begin
  // of the vector number and the interrut context. 
  // See kernel/arch/x86/x86-qemu/bsp.c
  asm volatile( "movl %%esp, %%eax \t\n"
      "1: \t\n"
      "addl $0x4, %%eax \t\n"
      "movl (%%eax), %%ebx \t\n"
      "cmp $0xffeeddcc, %%ebx \t\n"
      "jne  1b \t\n"
      "addl $0x4, %%eax \t\n"
      "movl (%%eax), %0\t\n"
      : "=r"(vector)
      :
      : "%eax", "%ebx"
     );
  
  (void)frame;
  
  C_dispatch_isr(vector);
  
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_ACK, vector );
  // recover frame, restore registers, place return eip;
  asm volatile( "movl %%esp, %%eax \t\n"
      "1: \t\n"
      "addl $0x4, %%eax \t\n"
      "movl (%%eax), %%ebx \t\n"
      "cmp $0xffeeddcc, %%ebx \t\n"
      "jne  1b \t\n"
      "addl $0x8, %%eax \t\n"
      "movl %%eax, %%esp \t\n"
      "popa \t\n"
      "ret	  \t\n"
      :
      :
      : "%eax", "%ebx", "%ecx", "%edx"
      );
}


/**
 * @brief Hello World!
 */
int user_hello( void )
{
  _BSP_Virtual_Char_write( "Hello POK world!\n");

  return 0;
}


/* Date:      06/10/2013
 * Author:    Philipp Eppelt
 * Purpose:   Implementation of the RTEMS virtualization layer.
 * Licensce:  see POK License.
 */


/**
 * \brief Initializes the console.
 *
 */
int
_BSP_Virtual_Console_init(void)
{
  return 0;
}

/**
 * \brief Reads a character from the console.
 */
char
_BSP_Virtual_Char_read(void)
{
  /* Console read is currently not supported by POK. -- phi 06/10/2013 */
  return -1;
}

/**
 * \brief Writes a character to the console.
 */
void
_BSP_Virtual_Char_write(char *c)
{
  printf("%s", c);
}



/* Startup functions */

int 
_BSP_Virtual_getworkspacearea( void )
{
  return 0;
}


/* Interrupts */

int
_CPU_Virtual_Irq_request( int vector )
{
  pok_ret_t ret = pok_syscall2( POK_SYSCALL_IRQ_REGISTER_HANDLER, vector, (uint32_t)&interrupt_middleman );
  if( ret != POK_ERRNO_OK )
  {
    printf( "Couldn't register handler\n");
    return -1;
  }
  
  return 0;
}

void
_CPU_Virtual_Irq_detach( int vector )
{
  pok_syscall1( POK_SYSCALL_IRQ_UNREGISTER_HANDLER, vector);
}

/**
 * \brief sets interrupt to _level 
 *
 * To assure proper usage, use _level previously returned by
 * _CPU_Virtual_Interrupts_disable!
 */
void
_CPU_Virtual_Interrupts_enable( int _level )
{
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_ENABLE, _level );
}
/**
 * \brief disables interrupts and returns previous level
 */ 
void
_CPU_Virtual_Interrupts_disable( int _level )
{
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_DISABLE, _level );
}

/**
 * \brief enables interrupt and disables them again; returned _level should be
 * the same as passed _level.
 */
void 
_CPU_Virtual_Interrupts_flash( int _level )
{
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_ENABLE, _level );
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_DISABLE, _level );
}

/**
 * @deprecated use _CPU_Virtual_Interrupts_enable with 0 as _level
 */
void
_CPU_Virtual_Interrupts_open( void )
{
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_ENABLE, 0 );
}

/**
 * @deprecated use _CPU_Virtual_Interrupts_enable with 0 as _level
 */
void
_CPU_Virtual_Interrupts_close( void )
{
  pok_syscall1( POK_SYSCALL_IRQ_PARTITION_DISABLE, 1 );
}

int
_CPU_Virtual_Interrupts_get_level( int _level )
{
  /* Really necessary?*/
  return 0;
}



/* Idle Thread */

/**
 * @brief Lowest priority thread, doing nothing, never returns;
 */

void
_CPU_Virtual_idle_thread( void )
{
  while(1);
}



/* Error handling */

void
_CPU_Virtual_exec_stop_error( int _error )
{
  printf( "!!! An ERROR occured: %i\n", _error );
}
