OUTPUT_FORMAT("elf32-i386");
OUTPUT_ARCH("i386")
ENTRY(_core_entry)

/* Multiboot spec uses 0x00100000 as a base */
PHYS_BASE = 0x00100000;
__REAL_BASE = 0x1000;

SECTIONS
{
	. = PHYS_BASE;

	.text : AT(ADDR(.text))
	{
		*(.multiboot)
		*(.text)
		*(*got*)
	}

	.rodata : 
	{
		*(.rodata);
        . = ALIGN(4);
	}

    /* We want to avoid this section being placed in low memory */
    .eh_frame : {
        *(.eh_frame*);
    }

    .text.realmode __REAL_BASE : AT(ADDR(.eh_frame) + SIZEOF(.eh_frame)) {
        *(.text.realmode);
        . = ALIGN(4);
    }

    __realmode_lma_start = LOADADDR(.text.realmode);
    __realmode_lma_end = __realmode_lma_start + SIZEOF(.text.realmode);

	.data (LOADADDR(.text.realmode) + SIZEOF(.text.realmode)): AT (LOADADDR(.text.realmode) + SIZEOF(.text.realmode))
	{
		*(.data*) *(.bss) *(COMMON)

		__archive_begin = .;
		*(.archive)
		__archive_end = .;
		__archive2_begin = .;
		*(.archive2)
        . = ALIGN(4);
		__archive2_end = .;
	}

	/* Nothing must have a VMA greater than __pok_end symbol address */
	__pok_end = . ;

    /DISCARD/ : {
        *(.note.gnu.property);
        *(.note.gnu.build-id);
        *(.comment);
    }
}
