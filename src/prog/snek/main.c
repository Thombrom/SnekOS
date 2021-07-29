#include "main.h"

void* _state_main_function;

void snek_main()
{
    _state_main_function = &snek_menu_main;
}

void snek_change_state(uint8_t _game_state, uint8_t _param)
{
    switch(_game_state)
    {
        case GAME_STATE_MENU:
            _state_main_function = &snek_menu_main;
            break;
        case GAME_STATE_GAME:
            _state_main_function = &snek_game_main;
            break;
        case GAME_STATE_END:
            _state_main_function = &snek_game_end_main;
            break;
    }
}
