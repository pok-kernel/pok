TOPDIR= ../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

CFLAGS += -I$(BSP)
LO_TARGET= sparc.lo

LO_OBJS= entry.o   \
         arch.o    \
         regwin.o  \
         space.o   \
         traps.o   \
         thread.o  \
         syscalls.o

LO_DEPS= $(BSP)/$(BSP).lo

all: $(LO_TARGET)

$(BSP)/$(BSP).lo::
	$(CD) $(BSP) && $(MAKE)

.PHONY: clean distclean depend all

clean: common-clean
	$(MAKE) -C leon3 $@
ifdef BSP
	$(CD) $(BSP) && $(MAKE) clean
endif

distclean: clean
	$(MAKE) -C leon3 $@
	$(RM) .depend.mk
ifdef BSP
	$(CD) $(BSP) && $(MAKE) distclean
endif

depend:
	$(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk
ifdef BSP
	$(CD) $(BSP) && $(MAKE) depend
endif

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk

