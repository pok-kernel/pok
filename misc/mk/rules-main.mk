-include .depend.mk

LDOPTS=-T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds -o $@ $(POK_PATH)/kernel/pok.lo 

assemble-partitions:
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[BIN] partitions.bin"
# padding to get aligned file size (needed for SPARC)
	for v in $(PARTITIONS); do \
		dd if=/dev/zero of=$$v oflag=append conv=notrunc bs=1 count=`echo "4 - (\`ls -l $$v | awk '{print $$5}'\` % 4)" | bc` > /dev/null 2> /dev/null;\
	done
	cat $(PARTITIONS) > partitions.bin
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi


$(TARGET): assemble-partitions
#	$(RM) -f cpio.c
#	$(TOUCH) cpio.c
#	$(CC) $(CONFIG_CFLAGS) -c cpio.c -o cpio.o
#	$(RM) -f cpio.c
#	$(OBJCOPY) --add-section .archive=$(ARCHIVE) cpio.o
	$(RM) -f sizes.c
	$(TOUCH) sizes.c
	$(ECHO) "#include <types.h>" >> sizes.c
	$(ECHO) "uint32_t part_sizes[] = {" >> sizes.c
	N=1 ; for v in $(PARTITIONS); do \
		if test $$N -eq 0; then $(ECHO) "," >> sizes.c ; fi ; N=0 ;\
		ls -l $$v|awk '{print $$5}' >> sizes.c ; \
	done
	$(ECHO) "};" >> sizes.c
	$(CC) $(CONFIG_CFLAGS) -I $(POK_PATH)/kernel/include -c sizes.c -o sizes.o
	$(OBJCOPY) --add-section .archive2=partitions.bin sizes.o
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[LD] $@"
	$(LD) $(LDFLAGS) -T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds -o $@ $(KERNEL) $(OBJS) sizes.o -Map $@.map
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

plop: assemble-partitions
	$(RM) -f sizes.c
	$(TOUCH) sizes.c
	$(ECHO) "#include <types.h>" >> sizes.c
	$(ECHO) "uint32_t part_sizes[] = {" >> sizes.c
	N=1 ; for v in $(PARTITIONS); do \
		if test $$N -eq 0; then $(ECHO) "," >> sizes.c ; fi ; N=0 ;\
		ls -l $$v|awk '{print $$5}' >> sizes.c ; \
	done
	$(ECHO) "};" >> sizes.c
	$(CC) $(CONFIG_CFLAGS) -I $(POK_PATH)/kernel/include -c sizes.c -o sizes.o
	$(OBJCOPY) --add-section .archive2=partitions.bin sizes.o
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[LD] $@"
	$(LD) $(LDFLAGS) -T $(POK_PATH)/misc/ldscripts/$(ARCH)/$(BSP)/kernel.lds -o pok.elf $(KERNEL) $(OBJS) sizes.o -Map $@.map
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
