TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=drivers.lo

LO_OBJS=	rtl8029.o

LO_DEPS=

include $(TOPDIR)/misc/mk/objdir.mk

all: $(LO_TARGET)

.PHONY: clean distclean depend all

distclean:

clean: common-clean

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk

