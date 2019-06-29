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

ifeq ($(TARGET_KERNEL),) # This variable should identify the path to the copied libpok.a file. Put a dummy value if empty
TARGET_KERNER = DUMMY_TARGET_KERNEL
endif

kernel: $(TARGET_KERNEL)

$(TARGET_KERNEL): $(DEPLOYMENT_HEADER)
	$(CD) $(POK_PATH)/kernel && $(MAKE) all

copy-kernel:
	$(CP) $(POK_PATH)/kernel/pok.lo $(shell pwd)/

kernel-clean:
	$(CD) $(POK_PATH)/kernel && $(MAKE) clean
	$(RM) $(shell pwd)/pok.lo
