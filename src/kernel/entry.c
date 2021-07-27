#include "../driver/screen_vga.h"
#include "../driver/serial.h"
#include "../driver/keyboard.h"

#include "idt.h"

uint8_t sample_keyboard_handler(struct key_event_t _key_event);

// Kernel Entrypoint
void _start() {
    idt_init();
    keyboard_activate();

    keyboard_register_event_handler(&sample_keyboard_handler);

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

uint8_t sample_keyboard_handler(struct key_event_t _key_event)
{
    if (!_key_event.key_code)
        return 0;

    screen_set_cursor_position(0, 0);
    screen_putchar('0');
    screen_putchar('x');

    char* lookup_table = "0123456789ABCDEF";
    uint8_t hex_offset_1 = (uint8_t)((_key_event.key_code >> 4) & 0x0F);
    uint8_t hex_offset_2 = (uint8_t)(_key_event.key_code & 0x0F);

    screen_putchar(lookup_table[hex_offset_1]);
    screen_putchar(lookup_table[hex_offset_2]);
    return 1;
}
