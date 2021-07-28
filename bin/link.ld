OUTPUT_FORMAT(binary)

ENTRY(_start)

INPUT
(
    build/obj/protected_mode.o
    build/obj/driver/screen_vga.o
    build/obj/driver/serial.o
    build/obj/driver/keyboard.o
    build/obj/kernel/entry.o
    build/obj/kernel/io.o
    build/obj/kernel/idt.o
    build/obj/kernel/pic.o
    build/obj/kernel/ps2.o
    build/obj/kernel/time.o
    build/obj/interrupt.o
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

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt =  .;
        . = . + 0x1000;
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
