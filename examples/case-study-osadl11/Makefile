BUILD = --models=model.aadl,software.aadl --no-run
TESTS = compile-x86 compile-ppc compile-sparc

ifeq ($(NODE), DISTRIBUTED)
BUILD = --models=model-distributed.aadl,software.aadl,rtl8029.aadl,runtime.aadl --no-run
endif

include $(POK_PATH)/misc/mk/examples.mk
