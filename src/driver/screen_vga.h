#pragma once

#include "../kernel/typedef.h"
#include "../kernel/io.h"

/*
 *  Background and foreground color definitions
 *  to be used together with 'screen_print_string_format'
 */

#define FG_BLACK            0x0
#define FG_BLUE             0x1
#define FG_GREEN            0x2
#define FG_CYAN             0x3
#define FG_RED              0x4
#define FG_MAGENTA          0x5
#define FG_BROWN            0x6
#define FG_LIGHT_GREY       0x7
#define FG_DARK_GREY        0x8
#define FG_LIGHT_BLUE       0x9
#define FG_LIGHT_GREEN      0xa
#define FG_LIGHT_CYAN       0xb
#define FG_LIGHT_RED        0xc
#define FG_LIGHT_MAGENTA    0xd
#define FG_LIGHT_BROWN      0xe
#define FG_WHITE            0xf

#define BG_BLACK            0x0 << 4
#define BG_BLUE             0x1 << 4
#define BG_GREEN            0x2 << 4
#define BG_CYAN             0x3 << 4
#define BG_RED              0x4 << 4
#define BG_MAGENTA          0x5 << 4
#define BG_BROWN            0x6 << 4
#define BG_LIGHT_GREY       0x7 << 4
#define BG_DARK_GREY        0x8 << 4
#define BG_LIGHT_BLUE       0x9 << 4
#define BG_LIGHT_GREEN      0xa << 4
#define BG_LIGHT_CYAN       0xb << 4
#define BG_LIGHT_RED        0xc << 4
#define BG_LIGHT_MAGENTA    0xd << 4
#define BG_LIGHT_BROWN      0xe << 4
#define BG_WHITE            0xf << 4

/*
 *  IO Port definitions
 */

#define CURSOR_POSITION_COMMAND_PORT    0x3D4
#define CURSOR_POSITION_DATA_PORT       0x3D5

#define CURSOR_POSITION_HIGH_BYTE_CMD   0x0E
#define CURSOR_POSITION_LOW_BYTE_CMD    0x0F

void screen_set_cursor_offset(uint16_t _offset);
void screen_set_cursor_position(uint16_t _w, uint16_t _h);

void screen_print_string(const char* _string);
void screen_print_string_f(const char* _string, uint8_t _format);

void screen_putchar(char _char);
void screen_putchar_f(char _char, uint8_t _format);

void screen_clear();
void screen_clear_f(uint8_t _format);

void screen_next_line();
void screen_cursor_disable();

extern uint16_t screen_cursor_offset;
