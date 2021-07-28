#include "../driver/screen_vga.h"
#include "../driver/serial.h"
#include "../driver/keyboard.h"

#include "idt.h"

uint8_t sample_keyboard_handler(struct key_event_t _key_event);

// Kernel Entrypoint
void _start() {
    idt_init();

    // Setup keyboard
    keyboard_activate();
    keyboard_register_event_handler(&sample_keyboard_handler);

    // Clear screen
    screen_clear_f(FG_WHITE | BG_BLACK);
    screen_set_cursor_position(0, 0);

    // Configure serial
    serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffers(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);

    while (1);
}

uint8_t sample_keyboard_handler(struct key_event_t _key_event)
{
    char ascii_char = key_event_to_ascii(_key_event);

    if (!ascii_char)
        return 0;

    if (ascii_char == '\n')
    {
        screen_next_line();
        return 1;
    }

    screen_putchar(ascii_char);
    return 1;
}
