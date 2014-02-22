export POK_PATH=../../../
export ARCH=x86
export BSP=x86-qemu

include $(POK_PATH)/misc/mk/config.mk
include $(POK_PATH)/misc/mk/common-$(ARCH).mk

TARGET=         $(shell pwd)/pok.elf
ARCHIVE= $(shell pwd)/partitions.cpio
KERNEL=kernel/kernel.lo
PARTITIONS=pr1/pr1.elf

all: build-kernel partitions $(TARGET)

partitions:
	$(CD) pr1 && $(MAKE) 

build-kernel:
	$(CD) kernel && $(MAKE)

clean: common-clean
	$(CD) kernel && $(MAKE) clean
	$(CD) pr1 && $(MAKE) clean

distclean: clean
	$(CD) kernel && $(MAKE) distclean
	$(CD) pr1 && $(MAKE) distclean

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
