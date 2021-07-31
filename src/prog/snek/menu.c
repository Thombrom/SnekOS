#include "menu.h"

/*
 *  Compilation unit values
 */

uint8_t keyboard_event_handler_vector;
uint8_t _keyboard_event_handler(struct key_event_t _key_event)
{
    if (_key_event.key_code == ENTER)
    {
        keyboard_unregister_event_handler(keyboard_event_handler_vector);
        snek_change_state(GAME_STATE_GAME, 0);
    }
}

/*
 *  Implementation
 */

void snek_menu_init()
{
    screen_clear_f(BG_BLACK | FG_GREEN);
    keyboard_event_handler_vector =
        keyboard_register_event_handler(&_keyboard_event_handler);
    snek_menu_draw();
}

void snek_menu_main()
{

}

void snek_menu_draw()
{
    uint8_t button_x_start = 27;
    uint8_t button_y_start = 10;

    screen_set_cursor_position(button_x_start, button_y_start + 0);
    screen_print_string("\xdb\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdf\xdb");
    screen_set_cursor_position(button_x_start, button_y_start + 1);
    screen_print_string("\xdb Press enter to begin \xdb");
    screen_set_cursor_position(button_x_start, button_y_start + 2);
    screen_print_string("\xdb\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdc\xdb");
    screen_set_cursor_position(0, 0);
}

void snek_menu_change_game()
{

}
