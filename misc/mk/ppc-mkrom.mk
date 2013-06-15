install: rom.bin

rom.bin: $(TARGET)
	$(OBJCOPY) -O binary $< $@

install-clean:
	$(RM) rom.bin

run: rom.bin
	$(QEMU) -L . -bios rom.bin rom.bin -M prep -serial /dev/stdout
