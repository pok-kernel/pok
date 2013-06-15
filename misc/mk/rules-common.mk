ECHO_FLAGS_ONELINE=-n

AR=$(AR_$(ARCH))
CC=$(CC_$(ARCH))
CXX=$(CXX_$(ARCH))
LD=$(LD_$(ARCH))
OBJCOPY=$(OBJCOPY_$(ARCH))
OBJDUMP=$(OBJDUMP_$(ARCH))
QEMU=$(QEMU_$(ARCH))
CONFIG_QEMU=$(CONFIG_QEMU_$(ARCH))
RANLIB=$(RANLIB_$(ARCH))
GNATMAKE=$(GNATMAKE_$(ARCH))

CFLAGS+=-D__POK_C__

ifneq ($(XCOV),) 
CFLAGS+=-DPOK_NEEDS_COVERAGE_INFOS
endif

ifneq ($(POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE),) 
CFLAGS+=-DPOK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE=1
endif



$(LO_TARGET): $(LO_DEPS) $(LO_OBJS)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[LD] $@ "
	$(LD) $(LDFLAGS) $(LDOPTS) -r $(LO_DEPS) $(LO_OBJS) -o $(LO_TARGET)
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

%.a: $(LO_DEPS)
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[AR] $@ "
	$(AR) $@ $(LO_DEPS)
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

%.o: %.S
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[CC] $< "
	$(CC) -c $(CFLAGS) -DASM_SOURCE=1 $< -o $@
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

%.o: %.c
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[CC] $< "
	$(CC) -c $(CFLAGS) $(COPTS) $< -o $@
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK"; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

%.o: %.adb %.ads
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[ADA] $< "
	$(CC) -c $(ADAFLAGS) $< -o $@
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK"; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

$(POK_PATH)/misc/mk/config.mk:
	cd $(POK_PATH) && $(MAKE) configure

-include .depend.mk

common-clean:
	$(RM) *~ .depend.mk
	$(RM) $(TARGET) $(LO_TARGET) $(LO_DEPS) $(LO_OBJS) $(OBJS) floppy.img partitions.lst partitions.cpio partitions.bin sizes.o sizes.c cpio.o *.elf.map



ifneq ($(CC),)
check-compiler:
	true
else
check-compiler:
	@echo "";
	@echo "Aieeeee, you don't have the compiler for $(ARCH)"
	@echo "-----------------------------------------------";
	@echo "";
	@echo "Please read the POK user guide and come back here !"
	@echo "";
	false
endif
