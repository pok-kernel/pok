TOPDIR=		../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	stdio.lo

LO_OBJS=	printf.o \
		getchar.o

LO_DEPS=

include $(TOPDIR)/misc/mk/objdir.mk

all: depend $(LO_TARGET)

.PHONY: clean distclean depend all

clean:
	$(RM) $(LO_TARGET)
	$(RM) $(LO_OBJS)

distclean:
	$(RM) $(LO_TARGET)
	$(RM) $(LO_OBJS)
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk
-include .depend.mk
