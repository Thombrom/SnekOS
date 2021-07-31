#include "game.h"

#define GAME_SIZE_X     19
#define GAME_SIZE_Y     19

uint8_t green_black_format = FG_GREEN | BG_BLACK;

uint8_t game_direction_vert;
uint8_t game_direction_hors;
uint8_t snake_length;

struct pos snake_positions[GAME_SIZE_X * GAME_SIZE_Y];
struct pos apple_pos;

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
    game_direction_vert = 1;
    game_direction_hors = 0;
    snake_length = 5;

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

    generate_apple_pos();
    snek_game_draw();
}

uint64_t before_time = 0;
void snek_game_main()
{
    uint64_t time = time_get_time_since_boot();
    if (!(time % 16) && (time != before_time))
    {
        before_time = time;
        snek_game_move();
    }
}

void generate_apple_pos()
{
    uint8_t x_pos, y_pos;

    // Keep generating until we have valid place
    // time changes
    while(1)
    {
        // Very simple pseudo RNG
        uint64_t seed = time_get_time_since_boot();
        x_pos = (seed % GAME_SIZE_X);
        y_pos = ((7 * seed) % GAME_SIZE_Y);

        for (uint8_t itt = 0; itt < snake_length; itt++)
        {
            struct pos link_pos = snake_positions[itt];
            if ((link_pos.x == x_pos) && (link_pos.y == y_pos))
                continue;
        }
        break;
    }

    apple_pos.x = x_pos;
    apple_pos.y = y_pos;
}

void snek_game_draw()
{
    // Draw snake
    for (uint8_t itt = 0; itt < snake_length; itt++)
    {
        struct pos snake_link = snake_positions[itt];
        snek_game_draw_link(snake_link.x, snake_link.y, 0);
    }

    // Draw apple
    snek_game_draw_link(apple_pos.x, apple_pos.y, FG_MAGENTA | BG_BLACK);

    // Draw score
    uint8_t digits[3] = { 0, 0, 0 };
    uint8_t score = snake_length - 5;   // Offset initial length

    digits[0] = score % 10;
    digits[1] = ((score - (score % 10)) / 10) % 10;
    digits[2] = ((score - (score % 100)) / 100) % 10;

    screen_set_cursor_position(65, 5);
    screen_print_string("Score: ");
    screen_putchar('0' + digits[2]);
    screen_putchar('0' + digits[1]);
    screen_putchar('0' + digits[0]);
}

void snek_game_move()
{
    uint8_t grow = 0;

    struct pos new_link_pos = snake_positions[snake_length - 1];
    new_link_pos.x += game_direction_hors;
    new_link_pos.y += game_direction_vert;
    snake_positions[snake_length] = new_link_pos;

    if (new_link_pos.x == apple_pos.x && new_link_pos.y == apple_pos.y)
        grow = 1;

    if (!snek_game_check_link_bounds(new_link_pos))
    {
        snek_game_change_state(GAME_STATE_END, 0);
        return;
    }

    if (!grow)
    {
        // Snake moves
        struct pos last_link = snake_positions[0];
        snek_game_draw_link(last_link.x, last_link.y, BG_BLUE | FG_BLACK);
        for (uint8_t itt = 0; itt < snake_length; itt++)
            snake_positions[itt] = snake_positions[itt + 1];
    }
    else
    {
        // Snake grows and apple respawns
        snake_length += 1;
        generate_apple_pos();
    }

    if (snake_length == GAME_SIZE_X * GAME_SIZE_Y)
    {   // Win condition
        snek_game_change_state(GAME_STATE_END, 1);
        return;
    }

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

void snek_game_change_state(uint8_t _state, uint8_t _args)
{
    keyboard_unregister_event_handler(game_keyboard_event_handler_vector);
    snek_change_state(_state, _args);
}
