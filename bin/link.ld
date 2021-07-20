OUTPUT_FORMAT(binary)

ENTRY(_start)

INPUT
(
    build/obj/protected_mode.o
    build/obj/driver/screen_vga.o
    build/obj/kernel/entry.o
    build/obj/kernel/io.o
)

OUTPUT
(
    build/bin/kernel.bin
)

SECTIONS
{
    . = 0x9000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}
