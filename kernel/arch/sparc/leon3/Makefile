include $(POK_PATH)/misc/mk/config.mk
-include $(POK_PATH)/misc/mk/common-$(ARCH).mk

LO_TARGET= leon3.lo

LO_OBJS= cons.o  \
         timer.o \
         bsp.o

LO_DEPS=

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean: common-clean

distclean: clean
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-kernel.mk

