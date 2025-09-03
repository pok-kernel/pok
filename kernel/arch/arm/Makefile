TOPDIR=		../../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=	arm.lo

LO_OBJS=	arch.o \
		space.o \
		thread.o \
		syscalls.o \
		exceptions.o \
		mpu.o \
		nvic.o

ifdef BSP
LO_DEPS=	$(BSP)/$(BSP).lo
endif

include $(TOPDIR)/misc/mk/objdir.mk

all: $(LO_TARGET)

ifdef BSP
$(LO_TARGET): $(OBJ_DIR)/$(BSP)/$(BSP).lo

$(OBJ_DIR)/$(BSP)/$(BSP).lo::
	$(CD) $(BSP) && $(MAKE)
endif
.PHONY: clean distclean depend all test

clean: common-clean
ifdef BSP
	$(CD) $(BSP) && $(MAKE) clean
endif

distclean: clean
	$(RM) .depend.mk
ifdef BSP
	$(CD) $(BSP) && $(MAKE) distclean
endif

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)
ifdef BSP
	$(CD) $(BSP) && $(MAKE) depend
endif

# Optional test target for checkmake compliance
test: all
	@echo "ARM architecture build test passed"

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-kernel.mk
