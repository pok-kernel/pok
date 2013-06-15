include $(POK_PATH)/misc/mk/config.mk
-include $(POK_PATH)/misc/mk/common-$(ARCH).mk

all: build

build:
	$(POK_PATH)/misc/pok-toolchain.pl $(BUILD)

clean:
	$(RM) *.o *.ali

distclean: clean
	$(RM) generated-code

run:
	$(MAKE) $@ -C generated-code

run-gdb:
	$(MAKE) run QEMU_MISC="$(QEMU_MISC) -S -s " -C generated-code

test: $(TESTS)

spoq:
ifeq ($(QEMU-SPOQ),)
	@echo "QEMU-SPOQ variable missing" >&2
	@exit 1
endif
	$(MAKE) run	QEMU=$(QEMU-SPOQ)		\
			QEMU_MISC="$(QEMU_MISC) --sys-profile $(POK_PATH)/misc/spoq.conf"

#
# test_target
#
# $(1) : Architecture
# $(2) : BSP
#
define test_target
	$(MKDIR) -p originals;						\
	$(CP) *.aadl originals;						\
	for model in $(shell find . -name "*.aadl");			\
	do								\
	    sed -i 's/\(POK::Architecture\).*;/\1 => $(1);/g' $$model;	\
	    sed -i 's/POK::BSP.*;/POK::BSP => $(2);/g' $$model;		\
	done;								\
	$(MAKE) distclean;						\
	$(ECHO) -n "`basename $(CURDIR)` on $(1)/$(2): ";		\
	if $(MAKE) all > /dev/null 2>&1;					\
	then								\
		$(ECHO) "SUCCESS";					\
		$(ECHO) -n 0 > .fail;					\
	else								\
		$(ECHO) "FAILURE";					\
		$(ECHO) -n 1 > .fail;					\
	fi;								\
	$(MAKE) distclean;						\
	$(MV) originals/*.aadl .;					\
	$(RM) originals;						\
	exit `cat .fail && $(RM) .fail`
endef

compile-x86:
	$(call test_target,x86,x86_qemu)

compile-sparc:
	$(call test_target,sparc,leon3)

compile-ppc:
	$(call test_target,ppc,prep)

#include $(POK_PATH)/misc/mk/rules-common.mk
#include $(POK_PATH)/misc/mk/rules-main.mk
#include $(POK_PATH)/misc/mk/install-rules.mk
