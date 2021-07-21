#include "screen_vga.h"

#define VGA_MEMORY (uint8_t*)0xb8000
#define VGA_HEIGHT 25
#define VGA_WIDTH  80

uint16_t screen_cursor_offset = 0;

void screen_set_cursor_offset(uint16_t _offset)
{
    out_byte(CURSOR_POSITION_COMMAND_PORT,  CURSOR_POSITION_LOW_BYTE_CMD);
    out_byte(CURSOR_POSITION_DATA_PORT,     (unsigned char)_offset & 0xFF);
    out_byte(CURSOR_POSITION_COMMAND_PORT,  CURSOR_POSITION_HIGH_BYTE_CMD);
    out_byte(CURSOR_POSITION_DATA_PORT,     (unsigned char)((_offset >> 8) & 0xFF));

    screen_cursor_offset = _offset;
}

void screen_set_cursor_position(uint16_t _w, uint16_t _h)
{
    uint16_t offset = _h * VGA_WIDTH + _w;
    screen_set_cursor_offset(offset);
}

void screen_print_string(const char* _string)
{
    uint8_t* char_ptr = (uint8_t*)_string;
    uint16_t pos_ptr  = screen_cursor_offset;

    while(*char_ptr != '\0')
        *(VGA_MEMORY + 2 * pos_ptr++) = *(char_ptr++);

    screen_set_cursor_offset(pos_ptr);
}

void screen_print_string_format(const char* _string, uint8_t _format)
{
    uint8_t* char_ptr = (uint8_t*)_string;
    uint16_t pos_ptr  = screen_cursor_offset;

    while(*char_ptr++ != '\0')
        *(VGA_MEMORY + 2 * pos_ptr++ + 1) = _format;

    screen_print_string(_string);
}

void screen_clear()
{
    for(uint16_t pos = 0; pos < VGA_WIDTH * VGA_HEIGHT; pos++)
        *(VGA_MEMORY + 2 * pos) = 0x20;
}

void screen_clear_format(uint8_t _format)
{
    for(uint16_t pos = 0; pos < VGA_WIDTH * VGA_HEIGHT; pos++)
        *(VGA_MEMORY + 2 * pos + 1) = _format;

    screen_clear();
}
