TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	libc.lo

LO_OBJS=

LO_DEPS=	stdio/stdio.lo string/string.lo misc/misc.lo stdlib/stdlib.lo


all: $(LO_TARGET)

stdio/stdio.lo::
	$(CD) stdio && $(MAKE)

stdlib/stdlib.lo::
	$(CD) stdlib && $(MAKE)

string/string.lo::
	$(CD) string && $(MAKE)

misc/misc.lo::
	$(CD) misc && $(MAKE)

.PHONY: clean distclean depend all

clean:
	$(RM) $(LO_TARGET)
	$(RM) $(LO_OBJS)
	$(CD) string && $(MAKE) clean
	$(CD) stdio && $(MAKE) clean
	$(CD) stdlib && $(MAKE) clean
	$(CD) misc && $(MAKE) clean

distclean:
	$(RM) $(LO_TARGET)
	$(RM) $(LO_OBJS)
	$(CD) string && $(MAKE) distclean
	$(CD) stdio && $(MAKE) distclean
	$(CD) stdlib && $(MAKE) distclean
	$(CD) misc && $(MAKE) distclean

depend:
	$(CD) string && $(MAKE) depend
	$(CD) stdio && $(MAKE) depend
	$(CD) stdlib && $(MAKE) depend
	$(CD) misc && $(MAKE) depend

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk
