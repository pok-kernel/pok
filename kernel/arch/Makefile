TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

ifdef ARCH
SUBDIRS:=$(ARCH)
else
SUBDIRS:=ppc sparc x86
endif

LO_TARGET=	arch.lo

LO_OBJS=		

LO_DEPS=	$(ARCH)/$(ARCH).lo

include $(TOPDIR)/misc/mk/objdir.mk

all: $(LO_TARGET)

$(OBJ_DIR)/$(ARCH)/$(ARCH).lo::
	$(CD) $(ARCH) && $(MAKE)

.PHONY: clean distclean depend all

clean: common-clean
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done

distclean: clean
	for dir in $(SUBDIRS); do $(MAKE) -C $$dir distclean; done

depend:
	$(CD) $(ARCH) && $(MAKE) depend

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk
