TOPDIR=		../../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	x86-qemu.lo

LO_OBJS=	cons.o \
			entry.o \
			bsp.o \
			pm.o \
			pic.o \
			pit.o

LO_DEPS=

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean: common-clean

distclean: clean
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk

