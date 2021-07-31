#include "game_end.h"

/*
 *  Compilation unit values
 */

uint8_t game_end_keyboard_event_handler_vector;
uint8_t _game_end_keyboard_event_handler(struct key_event_t _key_event)
{
    if (_key_event.key_code == ENTER)
    {
        keyboard_unregister_event_handler(game_end_keyboard_event_handler_vector);
        snek_change_state(GAME_STATE_GAME, 0);
    }
}

/*
 *  Implementation
 */

void snek_game_end_init(uint8_t _args)
{
    screen_clear_f(BG_BLACK | FG_GREEN);
    game_end_keyboard_event_handler_vector =
        keyboard_register_event_handler(&_game_end_keyboard_event_handler);
    snek_game_end_draw(_args);
}

void snek_game_end_main()
{

}

void snek_game_end_draw(uint8_t _args)
{
    if (_args) {
        screen_set_cursor_position(36, 5);
        screen_print_string_f("You won", FG_GREEN | BG_BLACK);
    } else {
        screen_set_cursor_position(35, 5);
        screen_print_string_f("Game over", FG_RED | BG_BLACK);
    }


    uint8_t button_x_start = 24;
    uint8_t button_y_start = 10;

    screen_set_cursor_position(button_x_start, button_y_start + 0);
    screen_print_string("\xdb\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdb");
    screen_set_cursor_position(button_x_start, button_y_start + 1);
    screen_print_string("\xdb Press enter to start over \xdb");
    screen_set_cursor_position(button_x_start, button_y_start + 2);
    screen_print_string("\xdb\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb");
    screen_set_cursor_position(0, 0);
}

