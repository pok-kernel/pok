OUTPUT_FORMAT("elf32-sparc");
OUTPUT_ARCH("sparc")
ENTRY(_pok_reset)

SECTIONS
{
  . = 0x40000000; /* Leon3 RAM address */

  .text :
  {
    *(.trapvector)
    *(.text)
  }
  .rodata :
  {
    *(.rodata)
    . = ALIGN(4);

    __archive_begin = .;
    *(.archive)
    __archive_end = .;
    __archive2_begin = .;
    *(.archive2)
    __archive2_end = .;
  }
  .data :
  {
    *(.data)
  }
  .bss :
  {
    *(.bss)
    . = ALIGN(16) + 0x100;
    _idlestack = .;
    . =  ALIGN(16) + 0x6000;
    _kstack = .;
    _end = _kstack;
  }
}

