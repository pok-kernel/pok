CFLAGS=$(CONFIG_CFLAGS) -nostdinc -iwithprefix include -fno-builtin -DPOK_ARCH_SPARC $(KIND_CFLAGS) $(GENERIC_FLAGS) -Wall -mno-app-regs -mcpu=v8 #-msoft-float
#LDFLAGS= /usr/local/lib/gcc/sparc-elf/4.3.3/v8/libgcc.a

