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


#ifndef __POK_X86_EVENT_H__
#define __POK_X86_EVENT_H__

#include <types.h>
#include <arch/x86/interrupt.h>

#include "gdt.h"

typedef enum e_idte_type
{
  IDTE_TASK = 5,
  IDTE_INTERRUPT = 6,
  IDTE_TRAP = 7
} e_idte_type;

typedef struct
{
  uint32_t	offset_low:16;
  uint32_t	segsel:16;
  uint32_t	res0:8;
  uint32_t	type:3;
  uint32_t	d:1;
  uint32_t	res1:1;
  uint32_t	dpl:2;
  uint32_t	present:1;
  uint32_t	offset_high:16;
} __attribute__((packed)) idt_entry_t;

#define EXCEPTION_DIVIDE_ERROR		0
#define EXCEPTION_DEBUG			1
#define EXCEPTION_NMI			2
#define EXCEPTION_BREAKPOINT		3
#define EXCEPTION_OVERFLOW		4
#define EXCEPTION_BOUNDRANGE		5
#define EXCEPTION_INVALIDOPCODE		6
#define EXCEPTION_NOMATH_COPROC		7
#define EXCEPTION_DOUBLEFAULT		8
#define EXCEPTION_COPSEG_OVERRUN	9
#define EXCEPTION_INVALID_TSS		10
#define EXCEPTION_SEGMENT_NOT_PRESENT	11
#define EXCEPTION_STACKSEG_FAULT	12
#define EXCEPTION_GENERAL_PROTECTION	13
#define EXCEPTION_PAGEFAULT		14
#define EXCEPTION_RESERVED		15
#define EXCEPTION_FPU_FAULT		16
#define EXCEPTION_ALIGNEMENT_CHECK	17
#define EXCEPTION_MACHINE_CHECK		18
#define EXCEPTION_SIMD_FAULT		19

void  pok_idt_set_gate(uint16_t index,
                       uint16_t segsel,
                       uint32_t offset,
                       e_idte_type t,
                       int dpl);
pok_ret_t pok_idt_init();
pok_ret_t pok_exception_init();
pok_ret_t pok_event_init();
pok_ret_t pok_syscall_init();

#endif /* !__POK_X86_EVENT_H__ */

