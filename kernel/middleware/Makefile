TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=middleware.lo

LO_OBJS=	portqueueingcreate.o \
			portqueueingsend.o \
			portqueueingreceive.o \
			portqueueingid.o \
			portqueueingstatus.o \
			portsamplingcreate.o \
			portsamplingid.o \
			portsamplingread.o \
			portsamplingwrite.o \
			portsamplingstatus.o \
			portvirtualid.o \
			portvirtualnbdestinations.o \
			portvirtualdestination.o \
			portvirtualgetglobal.o \
			portflushall.o \
			portinit.o \
			portcreate.o \
			portutils.o \
			queueinit.o \
			ressources.o 

LO_DEPS=

all: $(LO_TARGET)

.PHONY: clean distclean depend all

distclean:

clean: common-clean

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk

