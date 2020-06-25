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

    call main_ap

spin:
    hlt
    jmp spin


.code16
.section .data.realmode
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