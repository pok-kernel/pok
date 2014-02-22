OUTPUT_FORMAT("elf32-powerpc");
OUTPUT_ARCH("powerpc")
ENTRY(_pok_reset)

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  . = 0xfff00000;

  .text           :
  {
    *(.start)
    *(.text .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    __text_end = .;
  } = 0

  .rodata :
  {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
    . = ALIGN(4);

    __archive_begin = .;
    *(.archive)
    __archive_end = .;
    __archive2_begin = .;
    *(.archive2)
    __archive2_end = .;

    __rodata_end = .;
  } =0

  . = 0x10000;

  __sdata2_load = __rodata_end;
  .sdata2 : AT (__sdata2_load)
  {
    __sdata2_start = .;
    PROVIDE (_SDA2_BASE_ = 32768);
    *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
    . = ALIGN(4);
    __sdata2_end = .;
  }
  .sbss2 :
  {
    __sbss2_start = .;
    *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
    . = ALIGN(4);
    __sbss2_end = .;
  }
  __data_load = __sdata2_load + SIZEOF (.sdata2);
  .data : AT(__data_load)
  {
    __data_start = .;
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
    *(.data1)
    PROVIDE (_SDA_BASE_ = 32768);
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    . = ALIGN(4);
    __data_end = .;
  }
  .sbss :
  {
    __sbss_start = .;
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    __sbss_end = .;
  }
  .bss  :
  {
   __bss_start = .;
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   . = ALIGN(4);
   __bss_end = .;

   /* Allocate the main stack area.  Make sure it starts 16bytes aligned
      to accommodate any possible program expectation.  */
   _stack = ALIGN(16) + 0x1000;
   _end = _stack;
  }

  .eh_frame :
  {
  .eh_frame_start = .;
  *(.eh_frame*)
  . = ALIGN (4);
  .eh_frame_end = .;
  }

  . = 0xfffffffc;
  .reset : AT (0xfffffffc)
  {
   *(.reset)
  }

}
