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
		*(*got*)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data*) *(.bss) *(COMMON)

		__archive_begin = .;
		*(.archive)
		__archive_end = .;
		__archive2_begin = .;
		*(.archive2)
		__archive2_end = .;
	}

	/* Nothing must have a VMA greater than __pok_end symbol address */
	__pok_end = . ;
}
