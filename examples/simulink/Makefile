POK_PATH=/../../
-include ../../misc/mk/config.mk
-include ../../misc/mk/common-$(ARCH).mk

all:
	POK_PATH=`pwd`/../../ ../../misc/pok-toolchain.pl --models=model.aadl --no-run

compiletest: all

clean:
	$(RM) node_impl *.o

distclean: clean

