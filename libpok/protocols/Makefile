TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=protocols.lo

LO_DEPS=ceasar/ceasar.lo \
        blowfish/blowfish.lo \
		  des/des.lo

include $(TOPDIR)/misc/mk/objdir.mk

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean:
	$(CD) ceasar && $(MAKE) clean
	$(CD) blowfish && $(MAKE) clean
	$(CD) des && $(MAKE) clean

$(OBJ_DIR)/blowfish/blowfish.lo::
	$(CD) blowfish && $(MAKE)

$(OBJ_DIR)/ceasar/ceasar.lo::
	$(CD) ceasar && $(MAKE)

$(OBJ_DIR)/des/des.lo::
	$(CD) des && $(MAKE)

distclean:
	$(CD) ceasar && $(MAKE) distclean
	$(CD) blowfish && $(MAKE) distclean
	$(CD) des && $(MAKE) distclean
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk

