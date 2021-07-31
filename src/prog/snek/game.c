#include "game.h"

#define GAME_SIZE_X     19
#define GAME_SIZE_Y     19

uint8_t green_black_format = FG_GREEN | BG_BLACK;

uint8_t game_direction_vert = 1;
uint8_t game_direction_hors = 0;
uint8_t snake_length = 5;

struct pos snake_positions[GAME_SIZE_X * GAME_SIZE_Y];

void (*game_change_state)();

uint8_t game_keyboard_event_handler_vector;
uint8_t game_keyboard_event_handler(struct key_event_t _key_event)
{
    switch(_key_event.key_code)
    {
        case UP:
            game_direction_vert = -1;
            game_direction_hors = 0;
            break;
        case DOWN:
            game_direction_vert = 1;
            game_direction_hors = 0;
            break;
        case LEFT:
            game_direction_vert = 0;
            game_direction_hors = -1;
            break;
        case RIGHT:
            game_direction_vert = 0;
            game_direction_hors = 1;
    }
}

void snek_game_init()
{
    screen_clear_f(green_black_format);

    game_keyboard_event_handler_vector =
        keyboard_register_event_handler(&game_keyboard_event_handler);

    for (uint8_t itt = 0; itt < snake_length; itt++)
    {
        struct pos snake_link_position;
        snake_link_position.x = 10;
        snake_link_position.y = 6 + itt;

        snake_positions[itt] = snake_link_position;

    }

    screen_set_cursor_position(21, 2);
    for (uint8_t itt = 0; itt < GAME_SIZE_X * 2; itt++)
        screen_putchar_f('\xdf', green_black_format);

    screen_set_cursor_position(21, 22);
    for (uint8_t itt = 0; itt < GAME_SIZE_X * 2; itt++)
        screen_putchar_f('\xdc', green_black_format);

    for (uint8_t itt = 0; itt < GAME_SIZE_Y + 2; itt++)
    {
        screen_set_cursor_position(20, 2 + itt);
        screen_putchar_f('\xdb', green_black_format);
    }

    for (uint8_t itt = 0; itt < GAME_SIZE_Y + 2; itt++)
    {
        screen_set_cursor_position(59, 2 + itt);
        screen_putchar_f('\xdb', green_black_format);
    }

    snek_game_draw();
}

uint64_t before_time = 0;
void snek_game_main()
{
    uint64_t time = time_get_time_since_boot();
    if (!(time % 16) && (time != before_time))
    {
        snek_game_move();
        before_time = time;
    }
}

void snek_game_draw()
{
    // Draw snake
    for (uint8_t itt = 0; itt < snake_length; itt++)
    {
        struct pos snake_link = snake_positions[itt];
        snek_game_draw_link(snake_link.x, snake_link.y, 0);
    }
}

void snek_game_move()
{
    struct pos new_link_pos = snake_positions[snake_length - 1];
    new_link_pos.x += game_direction_hors;
    new_link_pos.y += game_direction_vert;
    snake_positions[snake_length] = new_link_pos;

    if (!snek_game_check_link_bounds(new_link_pos))
        snek_game_change_state(GAME_STATE_MENU, 0);

    struct pos last_link = snake_positions[0];
    snek_game_draw_link(last_link.x, last_link.y, BG_BLUE | FG_BLACK);
    for (uint8_t itt = 0; itt < snake_length; itt++)
        snake_positions[itt] = snake_positions[itt + 1];

    snek_game_draw();
}

uint8_t snek_game_check_link_bounds(struct pos _pos)
{
    if (_pos.x >= GAME_SIZE_X)
        return 0;

    if (_pos.y >= GAME_SIZE_Y)
        return 0;

    // Check self intersect
    for (uint8_t itt = 0; itt < snake_length; itt++)
    {
        struct pos link_pos = snake_positions[itt];
        if (link_pos.x == _pos.x && link_pos.y == _pos.y)
            return 0;
    }
    return 1;
}

void snek_game_draw_link(uint8_t _x, uint8_t _y, uint8_t _format)
{
    if (!_format)
        _format = BG_BLACK | ((_x + _y) % 2 ? FG_RED : FG_BROWN);

    uint8_t offset_x =  21 + _x * 2;
    uint8_t offset_y =  3 +  _y;

    screen_set_cursor_position(offset_x, offset_y);
    screen_putchar_f('\xdb', _format);
    screen_putchar_f('\xdb', _format);
}

void memcpy_(uint8_t* _src, uint8_t* _dst, uint64_t _size)
{
    for (uint64_t itt = 0; itt < _size; itt++)
        *(_dst++) = *(_dst++);
}

void snek_game_change_state(uint8_t _state, uint8_t _args)
{
    keyboard_unregister_event_handler(game_keyboard_event_handler_vector);
    game_change_state(_state, _args);
}
