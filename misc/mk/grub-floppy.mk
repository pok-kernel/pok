
ifdef MCOPY_x86

install: floppy.img

floppy.img: $(TARGET) menu.txt
	$(GZIP) -dc < $(POK_PATH)/misc/grub.img.gz > $@
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[MCOPY] menu.txt to $@ "
	$(MCOPY_x86) -i $@ menu.txt ::/boot/grub/
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
	$(MMD_x86) -i $@ ::/system
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[MCOPY] $< to $@ "
	$(MCOPY_x86) -i $@ $(TARGET) ::/system/pok.elf
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi
	$(RM) menu.txt
endif

menu.txt:
	$(ECHO) timeout 0 > $@
	$(ECHO) default 0 >> $@
	$(ECHO) title  PolyORB Kernel >> $@
	$(ECHO) "root   (fd0)" >> $@
	$(ECHO) kernel /system/pok.elf >> $@

install-clean:
	$(RM) -f floppy.img

#################
# NETWORK STUFF #
#################

ifdef MAC_ADDR
NETWORK_ARGS=-net nic,model=ne2k_pci,macaddr=$(MAC_ADDR) -net socket,$(QEMU_NETWORK_MODE)=127.0.0.1:1234
else
NETWORK_ARGS=
endif

#########################
# INSTRUMENTATION STUFF #
#########################

ifeq ($(INSTRUMENTATION),1)
QEMU_MISC += -serial /dev/stdout
QEMU_ENDCOMMAND = >instrumentation-output
CHEDDAR_EVENTS_FILE=cheddar-events.xml
CHEDDAR_ARCHI_FILE=cheddar-archi.xml

instrumentation-pre-run:

instrumentation-post-run:
	$(SLEEP) 15
	$(RM) -f $(CHEDDAR_EVENTS_FILE) $(CHEDDAR_ARCHI_FILE)
	$(KILL) `$(CAT) qemu.pid` ; exit 0
	#Handle events file
	$(CAT) $(POK_PATH)/misc/cheddar-events-header.xml > $(CHEDDAR_EVENTS_FILE)
	$(GREP) "\[INSTRUMENTATION\]\[CHEDDAR\]\[EVENTS\]" instrumentation-output | $(SED) -e 's/\[INSTRUMENTATION\]\[CHEDDAR\]\[EVENTS\]//g' >> $(CHEDDAR_EVENTS_FILE)
	$(ECHO) "</processor>"    >> $(CHEDDAR_EVENTS_FILE)
	$(ECHO) "</event_table>"  >> $(CHEDDAR_EVENTS_FILE)
	#Handle archi file now
	$(CAT) $(POK_PATH)/misc/cheddar-archi-header.xml > $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "<cheddar>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "<processors>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "        <processor>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "                <name>pokkernel</name>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "                <scheduler> HIERARCHICAL_CYCLIC_PROTOCOL </scheduler>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "                <network_link> No_Network </network_link>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "        </processor>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "</processors>" >> $(CHEDDAR_ARCHI_FILE)
	$(GREP) "\[INSTRUMENTATION\]\[CHEDDAR\]\[ARCHI\]" instrumentation-output | $(SED) -e 's/\[INSTRUMENTATION\]\[CHEDDAR\]\[ARCHI\]//g' >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "</tasks>" >> $(CHEDDAR_ARCHI_FILE)
	$(ECHO) "</cheddar>" >> $(CHEDDAR_ARCHI_FILE)

else

instrumentation-pre-run:

instrumentation-post-run:
endif

##############
# XCOV STUFF #
##############
ifdef XCOV
QEMU_MISC += -trace pok.elf.trace
QEMU_ENDCOMMAND += -serial /dev/stdout >xcov.output 2>&1

xcov-pre-run: pok.elf
	$(RM) coverage-report
	$(RM) -f pok.elf.trace
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[XCOV] Prepare $(TARGET) binary"
	$(XCOV) run -t prepare $(TARGET)
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

xcov-post-run:
	$(SLEEP) 15
	$(KILL) `$(CAT) qemu.pid` ; exit 0
	$(RM) -f *.html *.css
	$(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[XCOV] Output $(TARGET) results"
	$(XCOV) disp-routines pok.elf 2>/dev/null |$(GREP) -v pok_port_init > routines.list
	$(XCOV) coverage -c insn -a html+ --routine-list=routines.list $(TARGET).trace >/dev/null 2>&1; exit 0
	if [ -f index.html ]; then \
		$(MKDIR) coverage-report ;  \
		$(MKDIR) coverage-report/kernel ; \
		$(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; \
		$(MV) *.html *.css coverage-report/kernel ; \
	else \
		$(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO ; WILL PRODUCE AUTOMATICALLY A BUG REPORT FOR XCOV"; \
		make xcov-buildbugreport ; \
	fi
	npart=0 ; \
	for part in $(PARTITIONS); do \
		partname=`dirname $$part` ;  \
		loadaddr=`grep "\[XCOV\] Partition $$npart" xcov.output |awk -F'|' '{print $$4}'`  ; \
		echo "Launch xcov at start address $$loadaddr (partition $$partname)" ; \
		$(XCOV) coverage --level=insn -a html+ --exec=$$part --text-start=0x$$loadaddr $(TARGET).trace >/dev/null 2>&1  ; \
		if [ -f index.html ]; then \
			$(MKDIR) -p coverage-report/$$partname ; \
			$(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; \
			$(MV) *.html *.css coverage-report/$$partname ; \
		else \
			$(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO ; WILL PRODUCE AUTOMATICALLY A BUG REPORT FOR XCOV"; \
			make xcov-buildbugreport ; \
		fi ; \
		npart=$$(expr $$npart + 1) ; \
	done

xcov-buildbugreport:
	$(RM) -rf xcov-bugreport
	$(MKDIR) xcov-bugreport
	$(XCOV) coverage -c insn -a html+asm --routine-list=routines.list $(TARGET).trace > xcov-bugreport/cmd_output 2>&1 ; exit 0
	$(CP) -f pok.elf xcov-bugreport/
	$(CP) -f pok.elf.trace xcov-bugreport/
	$(CP) -f routines.list xcov-bugreport/
	tar cvvfz xcov-bugreport.tgz xcov-bugreport

else

xcov-pre-run:

xcov-post-run:
endif

##############
# SPOQ STUFF #
##############
ifdef SPOQ
QEMU_MISC += -spoq $(POK_PATH)/misc/spoq.conf 
QEMU_ENDCOMMAND=>spoq.output 2>&1

spoq-pre-run:

spoq-post-run:

else

spoq-pre-run:

spoq-post-run:
endif

################
# COMMON STUFF #
################

launch-run:
	$(ECHO) $(ECHO_FLAGS) "[QEMU] Start"
	$(QEMU) $(CONFIG_QEMU) $(QEMU_MISC) $(NETWORK_ARGS) -pidfile qemu.pid -hda fat:. -boot a $(QEMU_ENDCOMMAND) &
	if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi

run: xcov-pre-run spoq-pre-run instrumentation-pre-run launch-run xcov-post-run instrumentation-post-run spoq-post-run

#old code, deprecated
#	for v in $(PARTITIONS); do \
#      cd `dirname $$v` ; \
#	   $(XCOV) run -t prepare `basename $$v`; \
#	   $(ECHO) $(ECHO_FLAGS) "[QEMU] Start for partition $$v" ; \
#      ( $(QEMU) $(CONFIG_QEMU) -hda fat:.. -pidfile qemu.pid -boot a -trace `basename $$v`.trace & ) ; \
#      $(SLEEP) 40 ; \
#	   $(KILL) `$(CAT) qemu.pid` ; \
#		$(RM) -f qemu.pdf ; \
#	   $(MKDIR) ../coverage-report/`dirname $$v` ; \
#	   $(ECHO) $(ECHO_FLAGS) $(ECHO_FLAGS_ONELINE) "[XCOV] Output $$v results" ; \
##	   $(XCOV) --text-start=0x0010a34a --coverage=insn -a html+asm `basename $$v`.trace 2>/dev/null ; \
#	   $(XCOV) coverage --level insn -a html+asm --trace `basename $$v`.trace 2>/dev/null ; \
#	   if test $$? -eq 0; then $(ECHO) $(ECHO_FLAGS) $(ECHO_GREEN) " OK "; else $(ECHO) $(ECHO_FLAGS) $(ECHO_RED) " KO"; fi ; \
#	   $(MV) *.html *.css ../coverage-report/`dirname $$v` ; \
#      cd .. ; \
#   done

