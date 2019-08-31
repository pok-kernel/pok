TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=core.lo

LO_OBJS=	allocator.o \
			timecomputedeadline.o \
			timeget.o \
			eventlock.o \
			errno.o \
			eventunlock.o \
			eventcreate.o \
			eventwait.o \
			eventsignal.o \
			eventbroadcast.o \
			errorlog.o \
			errorget.o \
			errorraise.o \
			errorhandlercreate.o \
			errorhandlerworker.o \
			errorhandlersetready.o \
			errorignore.o \
			errorconfirm.o \
			main.o \
			mutexcreate.o \
			mutextrylock.o \
			mutexlock.o \
			mutexunlock.o \
			semcreate.o \
			semsignal.o \
			semstatus.o \
			semwait.o \
			threadcreate.o \
			threadattrinit.o \
			threadperiod.o \
			threadsleep.o \
			threadstatus.o \
			threaddelayedstart.o \
			threadid.o \
			threadpriority.o \
			threadresume.o

LO_DEPS=

include $(TOPDIR)/misc/mk/objdir.mk

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
-include .depend.mk
