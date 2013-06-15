instrumentation.o: instrumentation.c
syscall.o: syscall.c ../..//kernel/include/bsp.h \
 ../..//kernel/include/types.h ../..//kernel/include/arch/x86/types.h \
 ../..//kernel/include/errno.h ../..//kernel/include/libc.h \
 ../..//kernel/include/arch/x86/ioports.h \
 ../..//kernel/include/core/syscall.h \
 ../..//kernel/include/arch/x86/pci.h ../..//kernel/include/core/debug.h \
 ../..//kernel/include/core/partition.h \
 ../..//kernel/include/core/thread.h ../..//kernel/include/core/lockobj.h \
 ../..//kernel/include/arch.h ../..//kernel/include/arch/x86/spinlock.h \
 ../..//kernel/include/core/time.h ../..//kernel/include/core/error.h \
 ../..//kernel/include/middleware/port.h
time.o: time.c
error.o: error.c
partition.o: partition.c
kernel.o: kernel.c
loader.o: loader.c
boot.o: boot.c ../..//kernel/include/arch.h ../..//kernel/include/types.h \
 ../..//kernel/include/arch/x86/types.h ../..//kernel/include/errno.h \
 ../..//kernel/include/arch/x86/spinlock.h ../..//kernel/include/bsp.h \
 ../..//kernel/include/core/time.h ../..//kernel/include/core/thread.h \
 ../..//kernel/include/core/sched.h \
 ../..//kernel/include/core/partition.h \
 ../..//kernel/include/middleware/port.h \
 ../..//kernel/include/core/lockobj.h \
 ../..//kernel/include/middleware/queue.h \
 ../..//kernel/include/core/boot.h \
 ../..//kernel/include/core/instrumentation.h
sched.o: sched.c
debug.o: debug.c
thread.o: thread.c ../..//kernel/include/types.h \
 ../..//kernel/include/arch/x86/types.h ../..//kernel/include/arch.h \
 ../..//kernel/include/errno.h ../..//kernel/include/arch/x86/spinlock.h \
 ../..//kernel/include/core/debug.h ../..//kernel/include/core/error.h \
 ../..//kernel/include/core/thread.h ../..//kernel/include/core/sched.h \
 ../..//kernel/include/core/partition.h ../..//kernel/include/core/time.h \
 ../..//kernel/include/core/instrumentation.h
lockobj.o: lockobj.c
cons.o: cons.c
