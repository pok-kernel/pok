OUTPUT_FORMAT("elf32-i386");
OUTPUT_ARCH("i386")
ENTRY(_core_entry)

SECTIONS
{
	. = 0x100000;

	__pok_begin = . ;

	.text :
	{
		*(.multiboot)
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data) *(.bss) *(COMMON)

		__archive_begin = .;
		*(.archive)
		__archive_end = .;
		__archive2_begin = .;
		*(.archive2)
		__archive2_end = .;
	}

	__pok_end = . ;
}
