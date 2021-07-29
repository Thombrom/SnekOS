#pragma once

#include "../../kernel/typedef.h"

#include "game.h"
#include "menu.h"
#include "game_end.h"

void snek_main();

enum game_state_t {
    GAME_STATE_MENU = 1,
    GAME_STATE_GAME = 2,
    GAME_STATE_END  = 3,
};

void snek_change_state(uint8_t _game_state, uint8_t _param);

