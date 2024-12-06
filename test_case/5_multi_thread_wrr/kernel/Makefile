export POK_PATH=$(CURDIR)/../../../
export ARCH=x86
export BSP=x86-qemu
export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h

include $(POK_PATH)/misc/mk/config.mk

LO_TARGET = kernel.lo
LO_DEPS = pok.lo

all: kernel copy-kernel $(LO_TARGET)

.PHONY: clean distclean install all

clean: common-clean kernel-clean

include $(POK_PATH)/misc/mk/common-$(ARCH).mk
include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-kernel.mk
