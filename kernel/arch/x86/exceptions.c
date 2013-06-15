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

/**
 * \file       arch/x86/exceptions.c
 * \author     Julian Pidancet
 */

#if defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_ERROR_HANDLING)

#include <errno.h>
#include <arch.h>
#include <core/debug.h>
#include <core/error.h>
#include <core/partition.h>
#include <libc.h>
#include "event.h"

void		exception_divide_error();
void		exception_debug();
void		exception_nmi();
void		exception_breakpoint();
void		exception_overflow();
void		exception_boundrange();
void		exception_invalidopcode();
void		exception_nomath_coproc();
void		exception_doublefault();
void		exception_copseg_overrun();
void		exception_invalid_tss();
void		exception_segment_not_present();
void		exception_stackseg_fault();
void		exception_general_protection();
void		exception_pagefault();
void		exception_fpu_fault();
void		exception_alignement_check();
void		exception_machine_check();
void		exception_simd_fault();

static const struct
{
  uint16_t	vector;
  void		(*handler)(void);
}
exception_list[] =
{
  { EXCEPTION_DIVIDE_ERROR, exception_divide_error},
  { EXCEPTION_DEBUG, exception_debug},
  { EXCEPTION_NMI, exception_nmi},
  { EXCEPTION_BREAKPOINT, exception_breakpoint},
  { EXCEPTION_OVERFLOW, exception_overflow},
  { EXCEPTION_BOUNDRANGE, exception_boundrange},
  { EXCEPTION_INVALIDOPCODE, exception_invalidopcode},
  { EXCEPTION_NOMATH_COPROC, exception_nomath_coproc},
  { EXCEPTION_DOUBLEFAULT, exception_doublefault},
  { EXCEPTION_COPSEG_OVERRUN, exception_copseg_overrun},
  { EXCEPTION_INVALID_TSS, exception_invalid_tss},
  { EXCEPTION_SEGMENT_NOT_PRESENT, exception_segment_not_present},
  { EXCEPTION_STACKSEG_FAULT, exception_stackseg_fault},
  { EXCEPTION_GENERAL_PROTECTION, exception_general_protection},
  { EXCEPTION_PAGEFAULT, exception_pagefault},
  { EXCEPTION_FPU_FAULT, exception_fpu_fault},
  { EXCEPTION_ALIGNEMENT_CHECK, exception_alignement_check},
  { EXCEPTION_MACHINE_CHECK, exception_machine_check},
  { EXCEPTION_SIMD_FAULT, exception_simd_fault},
  { 0, NULL}
};

pok_ret_t pok_exception_init()
{
  int i;

  for (i = 0; exception_list[i].handler != NULL; ++i)
  {
    pok_idt_set_gate (exception_list[i].vector,
		                GDT_CORE_CODE_SEGMENT << 3,
                      (uint32_t) exception_list[i].handler,
                      IDTE_INTERRUPT,
                      3);
  }

  return (POK_ERRNO_OK);
}

#if defined (POK_NEEDS_DEBUG) && ! defined (POK_NEEDS_ERROR_HANDLING)
static void dump_registers (interrupt_frame *frame)
{
  printf ("ES: %x, DS: %x\n", frame->es, frame->ds);
  printf ("CS: %x, SS: %x\n", frame->cs, frame->ss);
  printf ("EDI: %x, ESI: %x\n", frame->edi, frame->esi);
  printf ("EBP: %x, ESP: %x\n", frame->ebp, frame->esp);
  printf ("EAX: %x, ECX: %x\n", frame->eax, frame->ecx);
  printf ("EDX: %x, EBX: %x\n", frame->edx, frame->ebx);
  printf ("EIP: %x, ErrorCode: %x\n", frame->eip, frame->error);
  printf ("EFLAGS: %x\n\n", frame->eflags);
}
#endif

INTERRUPT_HANDLER (exception_divide_error)
{
  (void) frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

#ifdef POK_NEEDS_DEBUG
  printf ("[KERNEL] Raise divide by zero error, current thread=%d\n", POK_SCHED_CURRENT_THREAD);
#endif

  pok_error_declare (POK_ERROR_KIND_NUMERIC_ERROR);
  pok_sched_activate_error_thread ();
#else
  pok_fatal ("Divide error");
#endif
}

INTERRUPT_HANDLER (exception_debug)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
  printf ("[KERNEL] Raise debug fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
  pok_sched_activate_error_thread ();
#else

   #ifdef POK_NEEDS_DEBUG
  dump_registers(frame);
  pok_fatal ("Debug fault");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_nmi)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception NMI fault\n");
   #endif

   pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
   pok_sched_activate_error_thread ();
#else

   #ifdef POK_NEEDS_DEBUG
   dump_registers(frame);
   pok_fatal ("NMI Interrupt");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_breakpoint)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception breakpoint fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
   dump_registers(frame);
   pok_fatal ("Breakpoint");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_overflow)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception overflow fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_STACK_OVERFLOW);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
   dump_registers(frame);
   pok_fatal ("Overflow");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_boundrange)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception bound range fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_STACK_OVERFLOW);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Bound range exceded");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_invalidopcode)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception invalid opcode fault, current thread: %d\n", POK_SCHED_CURRENT_THREAD);
   #endif

  pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Invalid Opcode");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_nomath_coproc)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception no math coprocessor fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
  pok_sched_activate_error_thread ();
#else

   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Invalid No Math Coprocessor");
   #endif

#endif
}

INTERRUPT_HANDLER_errorcode (exception_doublefault)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception double fault\n");
   #endif

  pok_partition_error (POK_SCHED_CURRENT_PARTITION, POK_ERROR_KIND_PARTITION_HANDLER);
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Double Fault");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_copseg_overrun)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception copseg overrun fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_MEMORY_VIOLATION);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Coprocessur Segment Overrun");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_invalid_tss)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception invalid tss fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_MEMORY_VIOLATION);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Invalid TSS");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_segment_not_present)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception segment not present fault %d\n", POK_SCHED_CURRENT_THREAD);
   #endif

  pok_error_declare (POK_ERROR_KIND_MEMORY_VIOLATION);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Segment Not Present");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_stackseg_fault)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception stack segment fault\n");
   #endif

   pok_error_declare (POK_ERROR_KIND_MEMORY_VIOLATION);
   pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Stack-Segment Fault");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_general_protection)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception general protection fault current thread=%d\n", POK_SCHED_CURRENT_THREAD);
   #endif

  pok_error_declare (POK_ERROR_KIND_ILLEGAL_REQUEST);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
   dump_registers(frame);
   pok_fatal ("General Protection Fault");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_pagefault)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)
   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception pagefault fault\n");
   #endif

   pok_error_declare (POK_ERROR_KIND_MEMORY_VIOLATION);
   pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Page Fault");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_fpu_fault)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception FPU fault\n");
   #endif

   pok_error_declare (POK_ERROR_KIND_HARDWARE_FAULT);
   pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Floating Point Exception");
   #endif
#endif
}

INTERRUPT_HANDLER_errorcode (exception_alignement_check)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception alignment fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_HARDWARE_FAULT);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("Bad alignement");
   #endif
#endif
}

INTERRUPT_HANDLER (exception_machine_check)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception machine check fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_HARDWARE_FAULT);
  pok_sched_activate_error_thread ();
#else
   #ifdef POK_NEEDS_DEBUG
      pok_fatal ("Machine check error");
      dump_registers(frame);
   #endif
#endif
}

INTERRUPT_HANDLER (exception_simd_fault)
{
  (void)frame;
#if defined (POK_NEEDS_PARTITIONS) && defined (POK_NEEDS_ERROR_HANDLING)

   #ifdef POK_NEEDS_DEBUG
   printf ("[KERNEL] Raise exception SIMD fault\n");
   #endif

  pok_error_declare (POK_ERROR_KIND_HARDWARE_FAULT);
  pok_sched_activate_error_thread ();
#else

   #ifdef POK_NEEDS_DEBUG
      dump_registers(frame);
      pok_fatal ("SIMD Fault");
   #endif

#endif
}

#endif

