#include "../driver/screen_vga.h"
#include "../driver/serial.h"

#include "idt.h"

// Kernel Entrypoint
void _start() {
    idt_load();

    screen_clear_f(FG_WHITE | BG_BLACK);
    screen_set_cursor_position(10, 3);

    const char* test_string = "This is a rather long string,\nand we will see whether\nlinebreaks work in\nthis";

    screen_print_string(test_string);

    serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffers(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);

    if (!serial_write(SERIAL_COM1_BASE, "Hello World!"))
    {
        screen_set_cursor_position(0, 0);
        screen_print_string("Serial write exited");
    }

    while (1);
}
