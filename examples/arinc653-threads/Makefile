BUILD = --arinc653 --models=model.aadl --no-run
TESTS = compile-ppc compile-x86 compile-sparc

include $(POK_PATH)/misc/mk/examples.mk

ada-build:
	$(MAKE) -C sample-ada

ada-run:
	$(MAKE) -C sample-ada run

ada-clean:
	$(MAKE) -C sample-ada clean
