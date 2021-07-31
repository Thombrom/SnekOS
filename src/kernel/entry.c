#include "../driver/keyboard.h"
#include "../driver/screen_vga.h"
#include "idt.h"
#include "time.h"

#include "../prog/snek/main.h"

// Kernel Entrypoint
void _start() {
    idt_init();
    keyboard_activate();
    time_init();

    // Boot main snek program
    snek_main();

    while (1);
}
