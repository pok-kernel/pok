.text

.globl _C_isr_handler
.globl update_tss


_ISR_Handler:
/* the IRQ vector number resides in EAX */
      push  %eax		      ; /* push vector number to stack */
      call  _C_isr_handler	      ; /* func expects the argument on the stack */
      pop   %ecx		      ; /* grab vector number from stack */
      
      /* if systemcall return value goes to eax after stack is reestablished:
       * After popa instruction */

/*      cmp   %ecx, 16		; *//* is this an PIC IRQ? */
/*      jl    .no_return_value	; *//* if yes, then there is no return value */
/*      movl  %eax, 40(%esp)      ;*/ /* if not, save the return value */
      
/*.no_return_value: */
      call  update_tss

      /* Cleanup stack*/

      addl  $4, %esp	      ; /* discard old stack pointer */
      pop   %es		      ;	/* remove interrupt_frame from stack */
      pop   %ds		      ;
      popa		      ;
      addl  $4, %esp	      ; /* remove interrupt_frame padding from stack */

      sti		      ; /* enable interrupts again */

      /* Acknowledging the interrupt must be done in the user defined handler
       * code */
      iret		      ; /* return from interrupt */

      
	

#define DISTINCT_INTERRUPT_ENTRY(_vector)	  \
	.p2align 4				; \
	.globl pok_irq_prologue_ ## _vector 	; \
pok_irq_prologue_ ## _vector:		  \
	cli			      ; \
	subl  $4, %esp		      ; \
	pusha			      ; \
	push  %ds		      ; \
	push  %es		      ; \
        push  %esp		      ; \
	mov   $0x10, %ax	      ; \
        mov   %ax, %ds		      ; \
        mov   %ax, %es		      ; \
	movl  $ _vector, %eax	      ; \
	jmp   _ISR_Handler	      ; \


DISTINCT_INTERRUPT_ENTRY(0)
DISTINCT_INTERRUPT_ENTRY(1)
DISTINCT_INTERRUPT_ENTRY(2)
DISTINCT_INTERRUPT_ENTRY(3)
DISTINCT_INTERRUPT_ENTRY(4)
DISTINCT_INTERRUPT_ENTRY(5)
DISTINCT_INTERRUPT_ENTRY(6)
DISTINCT_INTERRUPT_ENTRY(7)
DISTINCT_INTERRUPT_ENTRY(8)
DISTINCT_INTERRUPT_ENTRY(9)
DISTINCT_INTERRUPT_ENTRY(10)
DISTINCT_INTERRUPT_ENTRY(11)
DISTINCT_INTERRUPT_ENTRY(12)
DISTINCT_INTERRUPT_ENTRY(13)
DISTINCT_INTERRUPT_ENTRY(14)
DISTINCT_INTERRUPT_ENTRY(15)
