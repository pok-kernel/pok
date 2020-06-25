OUTPUT_FORMAT("elf32-i386");
OUTPUT_ARCH("i386")
ENTRY(_core_entry)

/* Multiboot spec uses 0x00100000 as a base */
PHYS_BASE = 0x00100000;
REAL_BASE = 0x1000;

SECTIONS
{
	. = PHYS_BASE;

    __physreal_diff = . - REAL_BASE;
    .realmode REAL_BASE : AT(ADDR(.realmode) + __physreal_diff) {
       
        __realmode_vma_start = .;

        
        __realmode_lma_start = LOADADDR(.realmode);
        *(.text.realmode);
        *(.data.realmode);
    }

    . += __physreal_diff;
    . = ALIGN(4);
    __realmode_vma_end = .;
    __realmode_secsize   = ((__realmode_vma_end)-(__realmode_vma_start));
    __realmode_secsize_l = __realmode_secsize>>2;
    __realmode_lma_end   = __realmode_vma_start + __physreal_diff + __realmode_secsize;

   


	.text : AT(ADDR(.text))
	{
		*(.multiboot)
		*(.text)
		*(*got*)
	}

    
    
	.rodata : 
	{
		*(.rodata);
	}

    /* We want to avoid this section being placed in low memory */
    .eh_frame : {
        *(.eh_frame*);
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

    /DISCARD/ : {
        *(.note.gnu.build-id);
        *(.comment);
    }
}
