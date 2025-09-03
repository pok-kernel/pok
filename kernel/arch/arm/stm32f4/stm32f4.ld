/*
 * STM32F4 Linker Script for POK ARM Cortex-M port
 * Memory layout for STM32F407VG (1MB Flash, 128KB SRAM)
 */

ENTRY(Reset_Handler)

/* Memory layout */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  SRAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
  CCMSRAM (rw) : ORIGIN = 0x10000000, LENGTH = 64K  /* Core Coupled Memory for DMA-free data */
}

/* Stack size definitions */
_stack_size = 0x1000;  /* 4KB stack - sufficient for ISR nesting, printf, context frames */

/* MSP Placement Note:
 * Ideally, MSP should be at top of SRAM (0x20000000 + 128K) for optimal memory layout.
 * However, current design uses mid-SRAM placement to maintain compatibility with
 * startup.S which defines its own .stack section and __StackTop symbol.
 * Future enhancement: Modify startup.S to use linker-provided stack location. */

SECTIONS
{
  /* Vector table goes to flash - must be 0x200 (512) byte aligned for proper VTOR operation */
  .isr_vector :
  {
    . = ALIGN(0x200);
    KEEP(*(.isr_vector))
    /* Note: No additional alignment needed - 512 bytes is already 8-byte aligned */
  } >FLASH

  /* Program code and constants */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  /* ARM exception unwinding */
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } >FLASH

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  /* Initialized data (copied from flash to RAM at startup) */
  .data :
  {
    . = ALIGN(8);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(8);
    _edata = .;
  } >SRAM AT >FLASH

  _sidata = LOADADDR(.data);

  /* Uninitialized data (zero-initialized) */
  .bss :
  {
    . = ALIGN(8);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(8);
    _ebss = .;

    /* FIXED: Heap base should be after BSS in SRAM, not in CCMSRAM
     * This ensures heap is DMA-accessible and in contiguous SRAM */
    _end = .;
    PROVIDE(end = .);
  } >SRAM

  /* Stack section with improved collision prevention
   * Place sufficient gap between heap and stack, with better size calculation */
  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    /* Add safety gap between heap and stack to reduce collision risk */
    . = . + 0x100;  /* 256-byte safety gap */
    PROVIDE(__StackLimit = .);
    /* Absorb .stack section from startup.S to prevent orphan */
    KEEP(*(.stack))
    . = . + _stack_size;
    . = ALIGN(8);
    _estack = .;  /* Stack top for runtime stack operations */
    PROVIDE(__StackTop = .);
  } >SRAM

  /* CCM SRAM section - fast access memory not accessible by DMA
   * Note: CCMSRAM should NOT contain heap as it's not DMA-accessible
   * and would cause issues with memory allocators expecting contiguous SRAM */
  .ccmsram (NOLOAD) :
  {
    . = ALIGN(8);
    _sccmsram = .;
    *(.ccmsram)
    *(.ccmsram*)
    . = ALIGN(8);
    _eccmsram = .;
  } >CCMSRAM

  /* Remove specific debugging sections instead of entire libraries */
  /DISCARD/ :
  {
    *(.comment)
    *(.debug*)
    *(.note*)
    *(.eh_frame*)
  }

  /* Note: Stack overflow protection should be implemented in runtime code
   * since linker ASSERT syntax varies between toolchains */
}
