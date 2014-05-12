#define STACK_SIZE 0x4000

.globl core_entry, _core_entry

.section .multiboot
  .align 4
multiboot_header:
  .long   0x1BADB002                      /* magic */
  .long   0x00000000                      /* flags */
  .long   - 0x00000000 - 0x1BADB002       /* check */
  .long	  multiboot_header
  .long	  _core_entry

.section .text

core_entry:
_core_entry:
	movl $(pok_stack + STACK_SIZE - 4), %ebp
	movl %ebp, %esp

	/* Set EFLAGS to 0 */
	pushl $0
	popf

	mov %eax, pok_multiboot_magic
	mov %ebx, pok_multiboot_info

	call pok_boot
loop:
	hlt
	jmp loop

.comm	pok_stack, STACK_SIZE

.comm	pok_multiboot_magic, 4
.comm	pok_multiboot_info, 4

