ifndef POK_PATH
export POK_PATH=$(CURDIR)/../../
endif

include $(POK_PATH)/misc/mk/config.mk

all: 
	POK_PATH=`pwd`/../../ ../../misc/pok-toolchain.pl --models=model.aadl --no-run
	$(OBJDUMP) -D generated-code/cpu/pok.elf > pok.dump
	$(OBJDUMP) -D generated-code/cpu/part1/part1.elf > part.dump

compiletest: all

build-model:

run:
	QEMU_MISC="-nographic" $(MAKE) -C generated-code run > debug-output

run-gdb:
	QEMU_MISC="-nographic -s -S -p 1234" $(MAKE) -C generated-code run 

gdb:
	gdb --eval-command="target remote :1234" generated-code/cpu/pok.elf

watch-output:
	tail -f debug-output

clean: common-clean
	$(RM) generated-code

distclean: clean

include $(POK_PATH)/misc/mk/rules-common.mk
include $(POK_PATH)/misc/mk/rules-main.mk
include $(POK_PATH)/misc/mk/install-rules.mk
