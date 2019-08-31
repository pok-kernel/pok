export POK_PATH=$(CURDIR)/../../
export ARCH=x86
export BSP=x86-qemu

include $(POK_PATH)/misc/mk/config.mk
include $(POK_PATH)/misc/mk/common-$(ARCH).mk

TARGET=$(shell pwd)/pok.elf
ARCHIVE=$(shell pwd)/partitions.cpio
PARTITIONS=pr1/pr1.elf pr2/pr2.elf
KERNEL=kernel/kernel.lo

all: build-regular

build-regular: build-kernel partitions $(TARGET)

compiletest: all

build-model:
	POK_PATH=`pwd`/../../ ../../misc/pok-toolchain.pl --models=model.aadl --no-run

build-kernel:
	$(CD) kernel && $(MAKE)

partitions:
	$(CD) pr1 && $(MAKE) 
	$(CD) pr2 && $(MAKE) 

clean: common-clean
	$(RM) node_impl
	$(CD) kernel && $(MAKE) clean
	$(CD) pr1 && $(MAKE) clean
	$(CD) pr2 && $(MAKE) clean

distclean: clean
	$(CD) kernel && $(MAKE) distclean
	$(CD) pr1 && $(MAKE) distclean
	$(CD) pr2 && $(MAKE) distclean

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
