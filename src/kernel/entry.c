#include "../driver/screen_vga.h"
#include "../driver/serial.h"

#include "idt.h"

void keyboard_interrupt_handler(struct interrupt_frame _frame);

// Kernel Entrypoint
void _start() {
    idt_init();
    register_interrupt_handler(&keyboard_interrupt_handler, 0x09);

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

void keyboard_interrupt_handler(struct interrupt_frame _frame)
{
    uint8_t scan_code;
    in_byte(0x60, &scan_code);

    uint8_t digits[3] = { 0, 0, 0 };
    uint8_t* scan_code_str = "000";

    digits[0] = (scan_code - scan_code % 100) / 100;
    digits[1] = (scan_code - digits[0] * 100 - scan_code % 10) / 10;
    digits[2] = scan_code % 10;

    scan_code_str[0] = 48 + digits[0];
    scan_code_str[1] = 48 + digits[1];
    scan_code_str[2] = 48 + digits[2];

    screen_set_cursor_position(0, 0);
    screen_print_string(scan_code_str);

    out_byte(0x20, 0x20);
    out_byte(0xa0, 0x20);
}
