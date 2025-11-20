/* Linker Script for LM3S6965EVB (Cortex-M3) */

/* Entry Point */
ENTRY(Reset_Handler)

/* Memory definitions */
MEMORY
{
    /* LM3S6965 has 256KB Flash starting at 0x0 */
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    /* LM3S6965 has 64KB RAM starting at 0x20000000 */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* The startup code goes first into FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Startup code */
        . = ALIGN(4);
    } >FLASH

    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
        _etext = .;        /* define a global symbol at end of code */
    } >FLASH

    /* Initialized data sections goes into RAM, load LMA copy after code */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end */
    } >RAM AT> FLASH

    /* Uninitialized data section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* define a global symbol at bss start */
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end */
    } >RAM

    /* User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        . = . + 0x400; /* 1KB min stack size */
        . = ALIGN(8);
    } >RAM
}
