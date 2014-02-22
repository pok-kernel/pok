TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	arch.lo

LO_OBJS=		

LO_DEPS=	$(ARCH)/$(ARCH).lo

all: $(LO_TARGET)

$(ARCH)/$(ARCH).lo::
	$(CD) $(ARCH) && $(MAKE)

.PHONY: clean distclean depend all

clean: common-clean
ifdef ARCH
	$(MAKE) -C $(ARCH) $@
else
	for dir in $(shell find -mindepth 1 -maxdepth 1 -type d ! -name ".svn");	\
	do										\
		$(MAKE) -C $$dir $@ || exit 1;						\
	done
endif

distclean: clean
ifdef ARCH
	$(MAKE) -C $(ARCH) $@
else
	for dir in $(shell find -mindepth 1 -maxdepth 1 -type d ! -name ".svn");	\
	do										\
		$(MAKE) -C $$dir $@ || exit 1;						\
	done
endif

depend:
	$(CD) $(ARCH) && $(MAKE) depend

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk
