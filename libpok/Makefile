include ../misc/mk/config.mk
-include ../misc/mk/common-$(ARCH).mk

LO_DEPS=	arch/arch.lo \
			core/core.lo \
			drivers/drivers.lo \
			middleware/middleware.lo \
			arinc653/arinc653.lo \
			libm/libm.lo \
			protocols/protocols.lo \
			libc/libc.lo

ifeq ($(strip $(ARCH)), )
all:
	@echo "";
	@echo "Aieeeee, you forgot to provide an ARCH variable"
	@echo "-----------------------------------------------";
	@echo "";
	@echo "Please note that pok Makefile build must be run from code generation directory."
	@echo "";
	@exit 0
else
all: .depend libpok.a
endif

libpok.a: $(LO_DEPS)

arch/arch.lo::
	$(CD) arch && $(MAKE)

core/core.lo::
	$(CD) core && $(MAKE)

middleware/middleware.lo::
	$(CD) middleware && $(MAKE)

arinc653/arinc653.lo::
	$(CD) arinc653 && $(MAKE)

libc/libc.lo::
	$(CD) libc && $(MAKE)

libm/libm.lo::
	$(CD) libm && $(MAKE)

drivers/drivers.lo::
	$(CD) drivers && $(MAKE)

protocols/protocols.lo::
	$(CD) protocols && $(MAKE)

.PHONY: all clean distclean depend

clean: common-clean
	$(CD) arch && $(MAKE) clean
	$(CD) core && $(MAKE) clean
	$(CD) drivers && $(MAKE) clean
	$(CD) libc && $(MAKE) clean
	$(CD) libm && $(MAKE) clean
	$(CD) middleware && $(MAKE) clean
	$(CD) arinc653 && $(MAKE) clean
	$(CD) protocols && $(MAKE) clean
	$(RM) $(LO_TARGET)

distclean:
	$(CD) arch && $(MAKE) distclean
	$(CD) core && $(MAKE) distclean
	$(CD) drivers && $(MAKE) distclean
	$(CD) libc && $(MAKE) distclean
	$(CD) libm && $(MAKE) distclean
	$(CD) middleware && $(MAKE) distclean
	$(CD) arinc653 && $(MAKE) distclean
	$(CD) protocols && $(MAKE) distclean
	$(RM) libpok.a
	$(RM) .depend

depend .depend:
	$(CD) core && $(MAKE) depend
	$(CD) libc && $(MAKE) depend
	$(CD) drivers && $(MAKE) depend
	$(CD) libm && $(MAKE) depend
	$(CD) middleware && $(MAKE) depend
	$(CD) protocols && $(MAKE) depend
	$(CD) arinc653 && $(MAKE) depend
	$(TOUCH) .depend

include ../misc/mk/rules-common.mk
include ../misc/mk/rules-partition.mk
