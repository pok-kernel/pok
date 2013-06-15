export ARCH=x86
export BSP=x86-qemu
export POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE=1
include $(POK_PATH)/misc/mk/config.mk
include $(POK_PATH)/misc/mk/common-$(ARCH).mk
TARGET=$(shell pwd)/pok.elf
PARTITIONS= part1/part1.elf part2/part2.elf

KERNEL=kernel/kernel.lo
all: build-kernel partitions $(TARGET)
build-kernel:
	$(CD) kernel && $(MAKE)

partitions:
	$(CD) part1&& $(MAKE)
	$(CD) part2&& $(MAKE)


clean: common-clean
	$(CD) kernel && $(MAKE) clean
	$(CD) part1&& $(MAKE) clean
	$(CD) part2&& $(MAKE) clean


distclean: clean
	$(CD) kernel && $(MAKE) distclean
	$(CD) part1&& $(MAKE) distclean
	$(CD) part2&& $(MAKE) distclean


include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
