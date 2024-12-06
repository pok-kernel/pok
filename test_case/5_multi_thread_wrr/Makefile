export POK_PATH=$(CURDIR)/../../
export ARCH=x86
export BSP=x86-qemu

include $(POK_PATH)/misc/mk/config.mk
include $(POK_PATH)/misc/mk/common-$(ARCH).mk

TARGET = $(shell pwd)/pok.elf
ARCHIVE = $(shell pwd)/partitions.cpio
PARTITIONS = prog/prog.elf
KERNEL = kernel/kernel.lo

all: clean build-regular

build-regular: build-kernel partitions $(TARGET)

compiletest: all

build-kernel:
	$(CD) kernel && $(MAKE)

partitions:
	$(CD) prog && $(MAKE) 

clean: common-clean
	$(RM) node_impl
	$(CD) kernel && $(MAKE) clean
	$(CD) prog && $(MAKE) clean

distclean: clean
	$(CD) kernel && $(MAKE) distclean
	$(CD) prog && $(MAKE) distclean

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
