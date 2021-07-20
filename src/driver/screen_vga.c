#include "screen_vga.h"

#define VGA_MEMORY (uint8_t*)0x8b000
#define VGA_HEIGHT 25
#define VGA_WIDTH  80

uint16_t screen_cursor_offset = 0;

void screen_set_cursor_offset(uint16_t _offset)
{
    out_byte(0x3D4, 0x0F);
    out_byte(0x3D5, (unsigned char)_offset & 0xFF);
    out_byte(0x3D4, 0x0E);
    out_byte(0x3D5, (unsigned char)((_offset >> 8) & 0xFF));

    screen_cursor_offset = _offset;
}

void screen_set_cursor_position(uint16_t _w, uint16_t _h)
{
    uint16_t offset = _h * VGA_WIDTH + _w;
    screen_set_cursor_offset(offset);
}
