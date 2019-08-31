include ../misc/mk/config.mk
-include ../misc/mk/common-$(ARCH).mk

LO_TARGET=pok.lo

LO_DEPS= core/core.lo \
        middleware/middleware.lo \
	drivers/drivers.lo \
        libc/libc.lo \
        arch/arch.lo

include ../misc/mk/objdir.mk

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
all: check-compiler .depend $(LO_TARGET)
endif

#ifeq ($(OBJ_DIR),)
#OBJ_DIR=.
#else
#OBJ_DIR:=$(OBJ_DIR)/$(notdir $(CURDIR))
#endif

#LO_OBJS := $(patsubst %.o,$(OBJ_DIR)/%.o,$(LO_OBJS))
#OBJS := $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
#LO_DEPS := $(patsubst %.lo,$(OBJ_DIR)/%.lo,$(LO_DEPS))

$(OBJ_DIR)/arch/arch.lo::
	$(CD) arch && $(MAKE)

$(OBJ_DIR)/core/core.lo::
	$(CD) core && $(MAKE)

$(OBJ_DIR)/libc/libc.lo::
	$(CD) libc && $(MAKE)

$(OBJ_DIR)/middleware/middleware.lo::
	$(CD) middleware && $(MAKE)

$(OBJ_DIR)/drivers/drivers.lo::
	$(CD) drivers && $(MAKE)

.PHONY: all clean distclean depend

clean: common-clean
	$(CD) arch && $(MAKE) clean
	$(CD) core && $(MAKE) clean
	$(CD) libc && $(MAKE) clean
	$(CD) middleware && $(MAKE) clean
	$(CD) drivers && $(MAKE) clean
	$(RM) $(LO_TARGET)

distclean: clean
	$(CD) arch && $(MAKE) distclean
	$(CD) core && $(MAKE) distclean
	$(CD) libc && $(MAKE) distclean
	$(CD) middleware && $(MAKE) distclean
	$(CD) drivers && $(MAKE) distclean
	$(RM) .depend

depend .depend:
	$(CD) arch && $(MAKE) depend
	$(CD) core && $(MAKE) depend
	$(CD) libc && $(MAKE) depend
	$(CD) middleware && $(MAKE) depend
	$(CD) drivers && $(MAKE) depend
	$(TOUCH) .depend

include ../misc/mk/rules-common.mk
include ../misc/mk/rules-kernel.mk
