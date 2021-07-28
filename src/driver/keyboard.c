#include "keyboard.h"

/*
 *  Data only relevant for this compilation unit
 */

keyboard_event_handler_t keyboard_event_handlers[KEYBOARD_EVENT_HANDLER_MAX];
uint8_t keyboard_key_state_table[256];

/*
 *  Implementation
 */


void keyboard_activate(void)
{
    uint8_t interrupt_vector = PIC_MASTER_START_INTERRUPT + 1;
    register_interrupt_handler(&keyboard_interrupt_handler, interrupt_vector);

    // Register key state handler
    keyboard_register_event_handler(&_keyboard_register_key_state);
}

uint8_t keyboard_register_event_handler(keyboard_event_handler_t _event_handler)
{
    for (uint8_t itt = 0; itt < KEYBOARD_EVENT_HANDLER_MAX; itt++)
    {
        if (!keyboard_event_handlers[itt])
        {
            keyboard_event_handlers[itt] = _event_handler;
            return itt + 1;
        }
    }

    return 0;
}

void keyboard_interrupt_handler(struct interrupt_frame _frame)
{

    uint8_t scan_codes[6];
    uint8_t index = 0;

    while(scan_codes[index++] = _keyboard_scancode_await());
    struct key_event_t key_event = _scan_code_to_key_event(scan_codes);

    keyboard_event_handler_t event_handler;
    for (uint8_t itt = 0; itt < KEYBOARD_EVENT_HANDLER_MAX; itt++)
    {
        event_handler = keyboard_event_handlers[itt];
        if(event_handler && event_handler(key_event))
            break;
    }
}

uint8_t keyboard_key_state(uint8_t _key_code)
{
    return keyboard_key_state_table[_key_code];
}

uint8_t _keyboard_register_key_state(struct key_event_t _key_event)
{
    uint8_t pressed = !(_key_event.flags & KEYBOARD_FLAG_RELEASE);
    keyboard_key_state_table[_key_event.key_code] = pressed;

    return 0;
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

uint8_t key_code_to_ascii_lower[256] = {
    0,      'a',    'b',    'c',        // 0x00
    'd',    'e',    'f',    'g',        // 0x04
    'h',    'i',    'j',    'k',        // 0x08
    'l',    'm',    'n',    'o',        // 0x0C
    'p',    'q',    'r',    's',        // 0x10
    't',    'u',    'v',    'w',        // 0x14
    'x',    'y',    'z',    '0',        // 0x18
    '1',    '2',    '3',    '4',        // 0x1C
    '5',    '6',    '7',    '8',        // 0x20
    '9',    0,      0,      0,          // 0x24
    0,      0,      0,      0,          // 0x28
    0,      0,      0,      0,          // 0x2C
    '!',    '\'',   '\'',    '.',       // 0x30
    '&',    '/',    '=',    '?',        // 0x34
    '@',    0,    '$',    '\\',         // 0x38
    0,      '\t',   0,      0,          // 0x3C
    0,      '-',    '\n',   ';',        // 0x40
    ',',    ' ',    0,      0,          // 0x44
    0,      0,      0,      0,          // 0x48
    0,      0,      0,      0,          // 0x4C
    '[',    ']',    '{',    '}',        // 0x50
    '(',    ')',    0,      0,          // 0x54
    0,      0,      0,      0,          // 0x58
    0,      0,      0,      0,          // 0x5C
    0,      0,      0,      0,          // 0x60
    0,      0,      0,      0,          // 0x64
    0,      0,      0,      0,          // 0x68
    0,      0,      0,      0,          // 0x6C
    0,      0,      0,      0,          // 0x70
    0,      0,      0,      0,          // 0x74
    0,      0,      0,      0,          // 0x78
    0,      0,      0,      0,          // 0x7C
    '0',    '1',    '2',    '3',        // 0x80
    '4',    '5',    '6',    '7',        // 0x84
    '8',    '9',    '+',    '-',        // 0x88
    '*',    '/',    '\n',   0,          // 0x8C
    0,      0,      0,      0,          // 0x90
};

uint8_t key_code_to_ascii_upper[256] = {
    0,      'A',    'B',    'C',        // 0x00
    'D',    'E',    'F',    'G',        // 0x04
    'H',    'I',    'J',    'K',        // 0x08
    'L',    'M',    'N',    'O',        // 0x0C
    'P',    'Q',    'R',    'S',        // 0x10
    'T',    'U',    'V',    'W',        // 0x14
    'X',    'Y',    'Z',    '0',        // 0x18
    '!',    '"',    '#',    0,          // 0x1C
    '%',    '&',    '/',    '(',        // 0x20
    ')',    0,      0,      0,          // 0x24
    0,      0,      0,      0,          // 0x28
    0,      0,      0,      0,          // 0x2C
    '!',    '\'',   '\'',    '.',       // 0x30
    '&',    '/',    '=',    '?',        // 0x34
    '@',    0,    '$',    '\\',         // 0x38
    0,      '\t',   0,      0,          // 0x3C
    0,      '-',    '\n',   ';',        // 0x40
    ',',    ' ',    0,      0,          // 0x44
    0,      0,      0,      0,          // 0x48
    0,      0,      0,      0,          // 0x4C
    '[',    ']',    '{',    '}',        // 0x50
    '(',    ')',    0,      0,          // 0x54
    0,      0,      0,      0,          // 0x58
    0,      0,      0,      0,          // 0x5C
    0,      0,      0,      0,          // 0x60
    0,      0,      0,      0,          // 0x64
    0,      0,      0,      0,          // 0x68
    0,      0,      0,      0,          // 0x6C
    0,      0,      0,      0,          // 0x70
    0,      0,      0,      0,          // 0x74
    0,      0,      0,      0,          // 0x78
    0,      0,      0,      0,          // 0x7C
    '0',    '1',    '2',    '3',        // 0x80
    '4',    '5',    '6',    '7',        // 0x84
    '8',    '9',    '+',    '-',        // 0x88
    '*',    '/',    '\n',   0,          // 0x8C
    0,      0,      0,      0,          // 0x90
};

char key_event_to_ascii(struct key_event_t _key_event)
{
    uint8_t flags = _key_event.flags;
    uint8_t* lookup_table = key_code_to_ascii_lower;

    if (flags & KEYBOARD_FLAG_RELEASE)
        return 0;

    if ((flags & KEYBOARD_FLAG_SHIFT ? 1 : 0) ^ (flags & KEYBOARD_FLAG_CAPSLOCK ? 1 : 0))
        lookup_table = key_code_to_ascii_upper;

    return lookup_table[_key_event.key_code];
}

