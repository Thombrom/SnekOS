#include "main.h"

uint8_t game_state;

void snek_main()
{
    screen_cursor_disable();
    snek_change_state(GAME_STATE_MENU, 0);

    while(1)
        switch(game_state)
        {
            case GAME_STATE_GAME:
                snek_game_main();
                break;
            case GAME_STATE_MENU:
                snek_menu_main();
        }
}

void snek_change_state(uint8_t _game_state, uint8_t _param)
{
    game_state = _game_state;
    switch(_game_state)
    {
        case GAME_STATE_GAME:
            snek_game_init();
            break;
        case GAME_STATE_MENU:
            snek_menu_init();
            break;
        case GAME_STATE_END:
            break;
    }
}
