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


#ifndef __POK_INTERRUPT_H__
#define __POK_INTERRUPT_H__


#include <types.h>

typedef struct
{
  uint32_t es;
  uint32_t ds;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t __esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  /* These are pushed by interrupt */
  uint32_t error;	/* Error code or padding */
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;

  /* Only pushed with privilege switch */
  /* (Check cs content to have original CPL) */
  uint32_t esp;
  uint32_t ss;
} interrupt_frame;

extern uint32_t pok_tss;

void update_tss (interrupt_frame* frame);

#define INTERRUPT_HANDLER(name)						\
void name (void);							\
void name##_handler(interrupt_frame* frame);				\
  asm (	    			      			      		\
      ".global "#name "			\n"				\
      "\t.type "#name",@function	\n"				\
      #name":				\n"				\
      "cli			\n"				\
      "subl $4, %esp			\n"				\
      "pusha				\n"				\
      "push %ds				\n"				\
      "push %es				\n"				\
      "push %esp			\n"				\
      "mov $0x10, %ax			\n"				\
      "mov %ax, %ds			\n"				\
      "mov %ax, %es			\n"				\
      "call " #name"_handler		\n"				\
      "call update_tss			\n"				\
      "addl $4, %esp			\n"				\
      "pop %es				\n"				\
      "pop %ds				\n"				\
      "popa				\n"				\
      "addl $4, %esp			\n"				\
      "sti			\n"				\
      "iret				\n"				\
      );								\
void name##_handler(interrupt_frame* frame)

#define INTERRUPT_HANDLER_errorcode(name)				\
void name (void);							\
void name##_handler(interrupt_frame* frame);			\
  asm (	    			      			      		\
      ".global "#name "			\n"				\
      "\t.type "#name",@function	\n"				\
      #name":				\n"				\
      "cli			\n"				\
      "pusha				\n"				\
      "push %ds				\n"				\
      "push %es				\n"				\
      "push %esp			\n"				\
      "mov $0x10, %ax			\n"				\
      "mov %ax, %ds			\n"				\
      "mov %ax, %es			\n"				\
      "call " #name"_handler		\n"				\
      "call update_tss			\n"				\
      "addl $4, %esp			\n"				\
      "pop %es				\n"				\
      "pop %ds				\n"				\
      "popa				\n"				\
      "addl $4, %esp			\n"				\
      "sti			\n"				\
      "iret				\n"				\
      );								\
void name##_handler(interrupt_frame* frame)

#define INTERRUPT_HANDLER_syscall(name)						\
int name (void);							\
void name##_handler(interrupt_frame* frame);				\
  asm (	    			      			      		\
      ".global "#name "			\n"				\
      "\t.type "#name",@function	\n"				\
      #name":				\n"				\
      "cli			\n"				\
      "subl $4, %esp			\n"				\
      "pusha				\n"				\
      "push %ds				\n"				\
      "push %es				\n"				\
      "push %esp			\n"				\
      "mov $0x10, %ax			\n"				\
      "mov %ax, %ds			\n"				\
      "mov %ax, %es			\n"				\
      "call " #name"_handler		\n"				\
      "movl %eax, 40(%esp)         \n" /* return value */  \
      "call update_tss			\n"				\
      "addl $4, %esp			\n"				\
      "pop %es				\n"				\
      "pop %ds				\n"				\
      "popa				\n"				\
      "addl $4, %esp			\n"				\
      "sti			\n"				\
      "iret				\n"				\
      );								\
void name##_handler(interrupt_frame* frame)


#endif /* !__POK_INTERRUPT_H__ */
