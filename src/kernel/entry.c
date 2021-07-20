#include "../driver/screen_vga.h"

// Kernel Entrypoint
void _start() {
    int* video_memory = (int*)0xb8000;
    *video_memory = 0x50505050;

    screen_set_cursor_position(40, 12);

    while (1);
}
