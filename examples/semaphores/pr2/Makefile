export POK_PATH=$(CURDIR)/../../../
export ARCH=x86
export BSP=x86-qemu
export DEPLOYMENT_HEADER=$(shell pwd)/deployment.h

include $(POK_PATH)/misc/mk/config.mk

TARGET=  	pr2.elf
OBJS=		main.o activity.o


.PHONY: clean distclean install all

all: libpok $(TARGET)

clean: common-clean libpok-clean

include $(POK_PATH)/misc/mk/common-$(ARCH).mk
include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-partition.mk
