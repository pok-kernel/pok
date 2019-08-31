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

ifeq ($(TARGET_LIBPOK),) # This variable should identify the path to the copied libpok.a file. Put a dummy value if empty
TARGET_LIBPOK = $(POK_PATH)/libpok/libpok.a
endif

# This target produces libpok and a new library consisting of the contents of libpok and the partition's object files.
libpok: $(TARGET_LIBPOK)

$(TARGET_LIBPOK): $(DEPLOYMENT_HEADER)
	$(CD) $(POK_PATH)/libpok && $(MAKE) all
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[AR] libpart.a "
	$(AR) -x $(TARGET_LIBPOK)
	$(AR) -csr $(TARGET_LIBPOK) *.lo
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
	rm *.lo

$(TARGET): $(OBJS) $(TARGET_LIBPOK)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[Assemble partition $@ "
	$(LD) $(LDFLAGS) -T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/partition.lds $+ -o $@ -L$(dir $(TARGET_LIBPOK)) -lpok -Map $@.map
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

libpok-clean:
	$(CD) $(POK_PATH)/libpok && $(MAKE) clean
	$(RM) $(TARGET_LIBPOK) $(shell pwd)/libpart.a 
