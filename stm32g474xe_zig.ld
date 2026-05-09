MEMORY
{
  CCMSRAM (xrw) : ORIGIN = 0x10000000, LENGTH = 32K
  RAM     (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH   (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
}

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    *(.ARM.extab*)
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  .ARM.exidx :
  {
    . = ALIGN(4);
    *(.ARM.exidx*)
    . = ALIGN(4);
  } > FLASH

  _sidata = LOADADDR(.data);

  .data : ALIGN(4)
  {
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  .bss (NOLOAD) : ALIGN(4)
  {
    _sbss = .;
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = .;
  } > RAM

  .ccmsram (NOLOAD) : ALIGN(4)
  {
    *(.ccmsram*)
  } > CCMSRAM

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
