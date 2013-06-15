OUTPUT_FORMAT("elf32-sparc");
OUTPUT_ARCH("sparc")
ENTRY(__pok_partition_start)

SECTIONS
{
  . = 0x0;
  __partition_begin = . ;
  .text :
  {
    . = ALIGN(4);
    *(.text)
  }
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
  }
  .data :
  {
    . = ALIGN(4);
    *(.data)
  }
  .bss :
  {
    . = ALIGN(4);
    *(.bss)
  }
  __partition_end = . ;
}
