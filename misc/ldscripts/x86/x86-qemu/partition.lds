OUTPUT_FORMAT("elf32-i386");
OUTPUT_ARCH("i386")
ENTRY(__pok_partition_start)

SECTIONS
{
	. = 0x0;

	__partition_begin = . ;

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data) *(.bss) *(COMMON)
	}

	__partition_end = . ;
}
