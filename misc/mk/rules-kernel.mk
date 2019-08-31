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

ifeq ($(TARGET_KERNEL),) # This variable should identify the path to the copied pok.lo file.
TARGET_KERNEL = $(POK_PATH)/kernel/pok.lo
endif

kernel: $(TARGET_KERNEL)

$(TARGET_KERNEL): $(DEPLOYMENT_HEADER)
	$(CD) $(POK_PATH)/kernel && $(MAKE) all

copy-kernel:
	$(CP) $(TARGET_KERNEL) $(shell pwd)/

kernel-clean:
	$(CD) $(POK_PATH)/kernel && $(MAKE) distclean
	$(RM) $(TARGET_KERNEL)
