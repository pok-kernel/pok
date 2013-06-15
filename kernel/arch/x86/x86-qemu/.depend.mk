pit.o: pit.c ../../../..//kernel/include/errno.h \
 ../../../..//kernel/include/bsp.h ../../../..//kernel/include/types.h \
 ../../../..//kernel/include/arch/x86/types.h \
 ../../../..//kernel/include/core/time.h \
 ../../../..//kernel/include/core/sched.h \
 ../../../..//kernel/include/arch/x86/ioports.h \
 ../../../..//kernel/include/core/syscall.h \
 ../../../..//kernel/include/arch/x86/interrupt.h pic.h pit.h
bsp.o: bsp.c ../../../..//kernel/include/errno.h \
 ../../../..//kernel/include/arch.h ../../../..//kernel/include/types.h \
 ../../../..//kernel/include/arch/x86/types.h \
 ../../../..//kernel/include/arch/x86/spinlock.h cons.h pm.h pit.h pic.h
pm.o: pm.c ../../../..//kernel/include/errno.h \
 ../../../..//kernel/include/arch/x86/multiboot.h \
 ../../../..//kernel/include/types.h \
 ../../../..//kernel/include/arch/x86/types.h pm.h
cons.o: cons.c ../../../..//kernel/include/errno.h \
 ../../../..//kernel/include/arch/x86/ioports.h \
 ../../../..//kernel/include/core/syscall.h \
 ../../../..//kernel/include/types.h \
 ../../../..//kernel/include/arch/x86/types.h \
 ../../../..//kernel/include/libc.h \
 ../../../..//kernel/include/core/debug.h \
 ../../../..//kernel/include/core/cons.h cons.h
debug.o: debug.c
pic.o: pic.c ../../../..//kernel/include/types.h \
 ../../../..//kernel/include/arch/x86/types.h \
 ../../../..//kernel/include/errno.h \
 ../../../..//kernel/include/arch/x86/ioports.h \
 ../../../..//kernel/include/core/syscall.h pic.h
entry.o: entry.S
