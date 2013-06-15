export POK_PATH=../../../../
export ARCH=x86
export BSP=x86-qemu
export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h

include $(POK_PATH)/misc/mk/config.mk

LO_TARGET = kernel.lo
LO_OBJS   = ports.o
LO_DEPS   = pok.lo

.PHONY: clean distclean install all

all: kernel copy-kernel $(LO_TARGET)

clean: common-clean

distclean:
	$(RM) .depend.mk

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-kernel.mk
