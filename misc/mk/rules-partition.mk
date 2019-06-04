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
TARGET_LIBPOK = DUMMY_TARGET_LIBPOK
endif

# This target produces libpok and a new library consisting of the contents of libpok and the partition's object files.
libpok: $(TARGET_LIBPOK)

$(TARGET_LIBPOK): $(DEPLOYMENT_HEADER)
	$(CD) $(POK_PATH)/libpok && $(MAKE) clean all
	$(CP) $(POK_PATH)/libpok/libpok.a `pwd`/
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[AR] libpart.a "
	$(AR) -x libpok.a
	$(AR) -csr libpart.a *.lo
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
	rm *.lo

$(TARGET): $(OBJS)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[Assemble partition $@ "
	$(LD) $(LDFLAGS) -T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/partition.lds $+ -o $@ -L`pwd`/ -lpok -Map $@.map
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

libpok-clean:
	$(CD) $(POK_PATH)/libpok && $(MAKE) clean
