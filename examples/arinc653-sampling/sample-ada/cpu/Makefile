export ARCH=x86
export BSP=x86-qemu
include $(POK_PATH)/misc/mk/config.mk
include $(POK_PATH)/misc/mk/common-$(ARCH).mk
TARGET=$(shell pwd)/pok.elf
PARTITIONS= pr2/pr2.elf pr1/pr1.elf

KERNEL=kernel/kernel.lo
all: build-kernel partitions $(TARGET)
build-kernel:
	$(CD) kernel && $(MAKE)

partitions:
	$(CD) pr2&& $(MAKE)
	$(CD) pr1&& $(MAKE)


clean: common-clean
	$(CD) kernel && $(MAKE) clean
	$(CD) pr2&& $(MAKE) clean
	$(CD) pr1&& $(MAKE) clean


distclean: clean
	$(CD) kernel && $(MAKE) distclean
	$(CD) pr2&& $(MAKE) distclean
	$(CD) pr1&& $(MAKE) distclean


include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
