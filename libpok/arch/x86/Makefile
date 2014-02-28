TOPDIR=../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	x86.lo

LO_OBJS=	arch.o \
		ioports.o \
		pci.o \
		syscall.o

LO_DEPS=	

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean: common-clean

distclean: clean
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)
	$(CD) $(BSP) && $(MAKE) depend

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk

