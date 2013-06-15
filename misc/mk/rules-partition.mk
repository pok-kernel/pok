ifeq ($(TOPDIR),)
CFLAGS += -I$(POK_PATH)/libpok/include -I.
else
CFLAGS += -I$(TOPDIR)/libpok/include -I.
endif

ifneq ($(LUSTRE_DIRECTORY),)
CFLAGS += -I$(LUSTRE_DIRECTORY)
endif

ifneq ($(DEPLOYMENT_HEADER),)
COPTS += -include $(DEPLOYMENT_HEADER)
endif

libpok: 
	$(CD) $(POK_PATH)/libpok && $(MAKE) distclean all
	$(CP) $(POK_PATH)/libpok/libpok.a `pwd`/

$(TARGET): $(OBJS)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[Assemble partition $@ "
	$(LD) $(LDFLAGS) -T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/partition.lds $+ -o $@ -L$(POK_PATH)/libpok -lpok -Map $@.map
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

libpok-clean:
	$(CD) $(POK_PATH)/libpok && $(MAKE) clean
