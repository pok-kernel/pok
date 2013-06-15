arch.o: arch.c ../../..//kernel/include/errno.h \
 ../../..//kernel/include/core/partition.h event.h \
 ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h \
 ../../..//kernel/include/arch/x86/interrupt.h gdt.h
syscalls.o: syscalls.c ../../..//kernel/include/errno.h \
 ../../..//kernel/include/core/debug.h \
 ../../..//kernel/include/core/partition.h \
 ../../..//kernel/include/core/syscall.h ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h gdt.h event.h \
 ../../..//kernel/include/arch/x86/interrupt.h
pci.o: pci.c
exceptions.o: exceptions.c
interrupt.o: interrupt.c ../../..//kernel/include/arch/x86/interrupt.h \
 ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h
event.o: event.c ../../..//kernel/include/libc.h \
 ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h \
 ../../..//kernel/include/errno.h ../../..//kernel/include/core/syscall.h \
 event.h ../../..//kernel/include/arch/x86/interrupt.h gdt.h sysdesc.h
space.o: space.c ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h \
 ../../..//kernel/include/errno.h ../../..//kernel/include/libc.h \
 ../../..//kernel/include/bsp.h ../../..//kernel/include/arch.h \
 ../../..//kernel/include/arch/x86/spinlock.h \
 ../../..//kernel/include/arch/x86/interrupt.h gdt.h tss.h space.h \
 thread.h
thread.o: thread.c ../../..//kernel/include/bsp.h \
 ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h \
 ../../..//kernel/include/errno.h ../../..//kernel/include/libc.h \
 ../../..//kernel/include/core/thread.h gdt.h thread.h
gdt.o: gdt.c ../../..//kernel/include/libc.h \
 ../../..//kernel/include/types.h \
 ../../..//kernel/include/arch/x86/types.h \
 ../../..//kernel/include/errno.h gdt.h sysdesc.h tss.h
