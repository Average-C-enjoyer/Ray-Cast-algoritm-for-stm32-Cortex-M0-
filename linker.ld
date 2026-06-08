MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .vector_table :
    {
        KEEP(*(.vector_table))
    } > FLASH

    . = ALIGN(4);

    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    _sidata = .;

    .data :
    {
        _sdata = .;

        *(.data*)

        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;

        *(.bss*)
        *(COMMON)

        _ebss = .;
    } > RAM
}
