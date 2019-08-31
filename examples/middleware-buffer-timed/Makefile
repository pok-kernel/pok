POK_PATH=$(CURDIR)/../../
include $(POK_PATH)/misc/mk/config.mk

compiletest: all

all:
	POK_PATH=`pwd`/../../ ../../misc/pok-toolchain.pl --models=model.aadl --no-run

compiletest: all

clean:
	$(RM) node_impl

distclean: clean

