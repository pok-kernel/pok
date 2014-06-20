TOPDIR=		../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk


LO_TARGET=	x86.lo

LO_OBJS=	arch.o		\
			gdt.o		\
			event.o		\
			thread.o	\
			space.o		\
			syscalls.o	\
			interrupt.o	\
			pci.o		\
			exceptions.o

ifeq ($(BSP),x86-qemu-vmm)

LO_OBJS+= interrupt_prologue.o 	\
	  hypercall.o	\
	  arch_vcpu.o

endif
LO_DEPS=	$(BSP)/$(BSP).lo

all: $(LO_TARGET)

$(BSP)/$(BSP).lo::
	$(CD) $(BSP) && $(MAKE)

.PHONY: clean distclean depend all

clean: common-clean
	$(MAKE) -C x86-qemu clean
	$(MAKE) -C x86-qemu-vmm clean
ifdef BSP
	$(CD) $(BSP) && $(MAKE) clean
endif

distclean: clean
	$(MAKE) -C x86-qemu distclean
	$(MAKE) -C x86-qemu-vmm distclean
	$(RM) .depend.mk
	$(RM) *.o
ifdef BSP
	$(CD) $(BSP) && $(MAKE) distclean
endif

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)
	$(CD) $(BSP) && $(MAKE) depend



include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk
