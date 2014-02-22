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


#ifndef __POK_X86_TSS_H__
#define __POK_X86_TSS_H__

#include <types.h>

typedef struct
{
   uint32_t back_link;
   uint32_t	esp0;
   uint32_t	ss0;
   uint32_t	esp1;
   uint32_t	ss1;
   uint32_t	esp2;
   uint32_t	ss2;
   uint32_t	cr3;
   uint32_t	eip;
   uint32_t	eflags;
   uint32_t	eax;
   uint32_t	ecx;
   uint32_t	edx;
   uint32_t	ebx;
   uint32_t	esp;
   uint32_t	ebp;
   uint32_t	esi;
   uint32_t	edi;
   uint32_t	es;
   uint32_t	cs;
   uint32_t	ss;
   uint32_t	ds;
   uint32_t	fs;
   uint32_t	gs;
   uint32_t	ldt;
   uint16_t	trace_trap;
   uint16_t	io_bit_map_offset;
} __attribute__((packed)) tss_t;

#endif /* !__POK_X86_TSS_H__ */

