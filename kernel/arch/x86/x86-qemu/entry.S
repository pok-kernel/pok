#define STACK_SIZE 0x4000

.globl core_entry, _core_entry, pok_stack

.section .multiboot
  .align 4
multiboot_header:
  .long   0x1BADB002                      /* magic */
  .long   0x00000000                      /* flags */
  .long   - 0x00000000 - 0x1BADB002       /* check */

.section .text

core_entry:
_core_entry:
	movl $(pok_stack + STACK_SIZE), %ebp
	movl %ebp, %esp

	/* Enable SSE instruction set */
	mov %cr0, %eax
	andb $0xFB, %al
	mov %eax, %cr0
	mov %cr4, %eax
	orw $(3 << 9), %ax
	mov %eax, %cr4

	/* Set EFLAGS to 0 */
	pushl $0
	popf

	mov %eax, pok_multiboot_magic
	mov %ebx, pok_multiboot_info

	call pok_boot
loop:
	hlt
	jmp loop

.comm	pok_stack, POK_CONFIG_NB_PROCESSORS*STACK_SIZE

.comm	pok_multiboot_magic, 4
.comm	pok_multiboot_info, 4

