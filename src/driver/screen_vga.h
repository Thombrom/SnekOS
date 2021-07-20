#pragma once

#include "../kernel/typedef.h"
#include "../kernel/io.h"

void screen_set_cursor_offset(uint16_t _offset);
void screen_set_cursor_position(uint16_t _w, uint16_t _h);

void screen_print_string(const char* _string);

extern uint16_t screen_cursor_offset;
