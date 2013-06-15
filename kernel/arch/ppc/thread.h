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


#ifndef __POK_PPC_THREAD_H__
#define __POK_PPC_THREAD_H__

#include <types.h>

typedef struct
{
  uint32_t sp;
  uint32_t unused_lr;

  uint32_t cr;
  uint32_t r2;
  uint32_t r13;
  uint32_t r14;
  uint32_t r15;

  uint32_t r16;
  uint32_t r17;
  uint32_t r18;
  uint32_t r19;
  uint32_t r20;
  uint32_t r21;
  uint32_t r22;
  uint32_t r23;
  uint32_t r24;
  uint32_t r25;
  uint32_t r26;
  uint32_t r27;
  uint32_t r28;
  uint32_t r29;
  uint32_t r30;
  uint32_t r31;

  uint32_t pad;

  /* Previous frame.  */
  uint32_t back_chain;
  uint32_t lr;
} context_t;

typedef struct
{
  uint32_t sp;
  uint32_t unused_lr;
  
  uint32_t cr; /* 8 */
  uint32_t r0;
  uint32_t r2; /* 16 */
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6; /* 32 */
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10; /* 48 */
  uint32_t r11;
  uint32_t r12;
  uint32_t r13;
  uint32_t ctr; /* 64 */
  uint32_t xer;
  uint32_t srr0;
  uint32_t srr1;

  /* Previous frame.  */
  uint32_t back_chain;
  uint32_t lr;

  /* For initial frame alignment.  */
  uint32_t pad0;
  uint32_t pad1;
} volatile_context_t;

uint32_t		pok_context_create(uint32_t id,
					   uint32_t stack_size,
					   uint32_t entry);

void			pok_context_switch(uint32_t* old_sp,
					   uint32_t new_sp);


#endif /* !__POK_PPC_THREAD_H__ */

