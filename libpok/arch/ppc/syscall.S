        .text
        .globl pok_syscall2
        .globl pok_syscall3
        .globl pok_syscall4
        .globl pok_syscall5
pok_syscall2:
pok_syscall3:
pok_syscall4:
pok_syscall5:
        sc
        brl
