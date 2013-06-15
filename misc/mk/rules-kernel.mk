ifeq ($(TOPDIR),)
CFLAGS += -I$(POK_PATH)/kernel/include -Werror
else
CFLAGS += -I$(TOPDIR)/kernel/include -Werror
endif

ifeq ($(INSTRUMENTATION),1)
CFLAGS += -DPOK_NEEDS_INSTRUMENTATION=1
endif

ifneq ($(DEPLOYMENT_HEADER),)
COPTS += -include $(DEPLOYMENT_HEADER)
endif

kernel:
	$(CD) $(POK_PATH)/kernel && $(MAKE) distclean all

copy-kernel:
	$(CP) $(POK_PATH)/kernel/pok.lo $(shell pwd)/

kernel-clean:
	$(CD) $(POK_PATH)/kernel && $(MAKE) clean
