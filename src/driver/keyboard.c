#include "keyboard.h"

void keyboard_activate(void)
{
    uint8_t interrupt_vector = PIC_MASTER_START_INTERRUPT + 1;
    register_interrupt_handler(&keyboard_interrupt_handler, interrupt_vector);
}

void keyboard_interrupt_handler(struct interrupt_frame _frame)
{

    uint8_t scan_codes[6];
    uint8_t index = 0;

    while(scan_codes[index++] = _keyboard_scancode_await());

    struct key_event_t key_event = _scan_code_to_key_event(scan_codes);
    screen_set_cursor_position(0, 0);
    if (key_event.key_code)
    {
        screen_putchar('0');
        screen_putchar('x');

        char* lookup_table = "0123456789ABCDEF";
        uint8_t hex_offset_1 = (uint8_t)((key_event.key_code >> 4) & 0x0F);
        uint8_t hex_offset_2 = (uint8_t)(key_event.key_code & 0x0F);

        screen_putchar(lookup_table[hex_offset_1]);
        screen_putchar(lookup_table[hex_offset_2]);
    }
}

uint8_t _keyboard_scancode_await()
{
    uint8_t attempts = 0;
    while(attempts++ < KEYBOARD_KEY_AWAIT_CYCLES && !ps2_outbuffer_full());

    if (!ps2_outbuffer_full())
        return 0;

    in_byte(0x60, &attempts);
    return attempts;
}

uint8_t one_key_codes[128] = {
    NONE,   ESC,    D1,     D2,
    D3,     D4,     D5,     D6,
    D7,     D8,     D9,     D0,
    MINUS,  EQUAL,  BACKSPACE,  TAB,
    Q,      W,      E,      R,
    T,      Y,      U,      I,
    O,      P,      LEFT_BRACKET,  RIGHT_BRACKET,
    ENTER,  CTRL_LEFT,  A,  S,
    D,      F,      G,      H,
    J,      K,      L,      SEMICOLON,
    SINGLEQUOTE,    BACKTICK,   SHIFT_LEFT, BACKSLASH,
    Z,      X,      C,      V,
    B,      N,      M,      COMMA,
    PERIOD, SLASH,  SHIFT_RIGHT,    KP_MUL,
    ALT_LEFT,   SPACE,  CAPSLOCK,   F1,
    F2,     F3,     F4,     F5,
    F6,     F7,     F8,     F9,
    F10,    NUMLOCK,    SCROLLOCK,  KP7,
    KP8,    KP9,    KP_SUB,     KP4,
    KP5,    KP6,    KP_ADD,     KP1,
    KP2,    KP3,    KP0,    NONE,
    NONE,   NONE,   NONE,   F11,
    F12,    NONE,   NONE,   NONE,
};

uint8_t two_key_codes[128] = {
    NONE,   NONE,   NONE,   NONE,           // 0x00
    NONE,   NONE,   NONE,   NONE,           // 0x04
    NONE,   NONE,   NONE,   NONE,           // 0x08
    NONE,   NONE,   NONE,   NONE,           // 0x0C
    NONE,   NONE,   NONE,   NONE,           // 0x10
    NONE,   NONE,   NONE,   NONE,           // 0x14
    NONE,   NONE,   NONE,   NONE,           // 0x18
    KP_ENTER,   CTRL_RIGHT,   NONE,   NONE, // 0x1C
    NONE,   NONE,   NONE,   NONE,           // 0x20
    NONE,   NONE,   NONE,   NONE,           // 0x24
    NONE,   NONE,   NONE,   NONE,           // 0x28
    NONE,   NONE,   NONE,   NONE,           // 0x2C
    NONE,   NONE,   NONE,   NONE,           // 0x30
    NONE,   KP_DIV, NONE,   NONE,           // 0x34
    ALT_RIGHT,  NONE,   NONE,   NONE,       // 0x38
    NONE,   NONE,   NONE,   NONE,           // 0x3C
    NONE,   NONE,   NONE,   NONE,           // 0x40
    NONE,   NONE,   NONE,   HOME,           // 0x44
    UP,     PAGE_UP, NONE,  LEFT,           // 0x48
    NONE,   RIGHT,  NONE,   END,            // 0x4C
    DOWN,   PAGE_DOWN,  INSERT, DELETE,     // 0x50
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
    NONE,   NONE,   NONE,   NONE,
};

struct key_event_t _scan_code_to_key_event(uint8_t* _scan_codes)
{
    uint8_t* scan_code_lookup = one_key_codes;
    uint8_t  offset = _scan_codes[0];
    uint8_t  flags = 0;

    if (_scan_codes[0] == 0xE0)
    {
        scan_code_lookup = two_key_codes;
        offset = _scan_codes[1];
    }

    // Check for PRESS/RELEASE flag
    if (offset >= 0x80)
    {
        offset -= 0x80;
        flags |= KEYBOARD_FLAG_RELEASE;
    }

    struct key_event_t key_event;
    key_event.key_code  = scan_code_lookup[offset];
    key_event.flags     = flags;

    return key_event;
}
