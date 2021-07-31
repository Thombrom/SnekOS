#pragma once

#include "main.h"

void snek_game_init();
void snek_game_main();
void snek_game_draw();

struct pos {
    uint8_t x;
    uint8_t y;
};

void snek_game_draw_link(uint8_t _x, uint8_t _y, uint8_t _format);
void snek_game_move();
uint8_t snek_game_check_link_bounds(struct pos _pos);
void snek_game_change_state(uint8_t _state, uint8_t _args);

void memcpy_(uint8_t* _src, uint8_t* _dst, uint64_t _size);
