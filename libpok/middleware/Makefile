TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=middleware.lo

LO_OBJS=	blackboardcreate.o \
			blackboardclear.o \
			blackboarddisplay.o \
			blackboardid.o \
			blackboardstatus.o \
			blackboardinit.o \
			blackboardread.o \
			bufferinit.o \
			bufferid.o \
			bufferstatus.o \
			buffersend.o \
			bufferreceive.o \
			buffercreate.o \
			portsamplingcreate.o \
			portsamplingread.o \
			portsamplingwrite.o \
			portqueueingcreate.o \
			portqueueingreceive.o \
			portqueueingsend.o \
			portvirtualcreate.o \
			portvirtualdestination.o \
			portvirtualnbdestinations.o \
			portvirtualgetglobal.o \
			ressources.o

LO_DEPS=

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean:
	$(RM) $(LO_OBJS)

distclean:
	$(RM) $(LO_OBJS) $(LO_TARGET)
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk

