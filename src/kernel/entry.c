#include "../driver/screen_vga.h"
#include "idt.h"
#include "time.h"

#include "../prog/snek/main.h"

uint8_t sample_keyboard_handler(struct key_event_t _key_event);

// Kernel Entrypoint
void _start() {
    idt_init();
    keyboard_activate();
    time_init();

    // Boot main snek program
    snek_main();

    while (1);
}
