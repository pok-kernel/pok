#define STACK_SIZE 0x4000

.globl lapic_address

.code16
.section .text.realmode
enter_protected_mode:
    # clear interrupts
    cli

    lgdt (gdt32info)

    mov %cr0, %eax
    or $1, %al    # set protected mode bit
    mov %eax, %cr0

.intel_syntax
    jmp 0x08:protected_mode
.att_syntax 

.code32
protected_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    /* Enable SSE instruction set */
	mov %cr0, %eax
	andb $0xFB, %al
	mov %eax, %cr0
	mov %cr4, %eax
	orw $(3 << 9), %ax
	mov %eax, %cr4

    /* Allocate a stack to the AP:  pok_stack + LAPIC_ID * STACK_SIZE */
    movl lapic_address, %eax
    movl 0x20(%eax), %eax
    shrl $24, %eax
    movl $(STACK_SIZE), %ebx
    mull %ebx
    movl (stack_position), %ebx
    addl %ebx, %eax
    movl %eax, %ebp
	movl %ebp, %esp

    call main_ap

spin:
    hlt
    jmp spin

stack_position:
    .4byte pok_stack + STACK_SIZE

   .align 4
gdt32info:
   .word gdt32_end - gdt32 - 1  # last byte in table
   .word gdt32                  # start of table

gdt32:
    # entry 0 is always unused
    .quad 0
codedesc:
    .byte 0xff
    .byte 0xff
    .byte 0
    .byte 0
    .byte 0
    .byte 0x9a
    .byte 0xcf
    .byte 0
datadesc:
    .byte 0xff
    .byte 0xff
    .byte 0
    .byte 0
    .byte 0
    .byte 0x92
    .byte 0xcf
    .byte 0
gdt32_end: