#pragma once

#include "../kernel/io.h"
#include "../kernel/idt.h"
#include "../kernel/pic.h"
#include "../kernel/ps2.h"

#include "../driver/screen_vga.h"

#define KEYBOARD_KEY_AWAIT_CYCLES   30
#define KEYBOARD_EVENT_HANDLER_MAX  8

#define KEYBOARD_FLAG_RELEASE   1 << 0
#define KEYBOARD_FLAG_SHIFT     1 << 1
#define KEYBOARD_FLAG_ALT       1 << 2
#define KEYBOARD_FLAG_CTRL      1 << 3
#define KEYBOARD_FLAG_NUMLOCK   1 << 4
#define KEYBOARD_FLAG_CAPSLOCK  1 << 5
#define KEYBOARD_FLAG_SCROLLOCK 1 << 6

/*
 *  keyboard_activate
 *
 *  Activation function for the
 *  keyboard
 *
 *  Sets up relevant interrupt handler.
 *  Should be called after IDT setup
 */

void keyboard_activate(void);

/*
 *  keyboard_interrupt_handler
 *
 *  Interrupt handler for the keyboard
 */

void keyboard_interrupt_handler(struct interrupt_frame _frame);

/*
 *  keyboard_register_event_handler
 *
 *  Allows you to register an event handler for the
 *  keyboard. The handler should return truthy value
 *  if the event was handled. That stops it from passing
 *  on to the next registered handler
 */

struct key_event_t;
typedef uint8_t (*keyboard_event_handler_t)(struct key_event_t);

uint8_t keyboard_register_event_handler(keyboard_event_handler_t _event_handler);

/*
 *  key_state
 *
 *  Query key states on the go
 *  providing the key code.
 *
 *  A falsy value represents non-pressed
 *  and a truthy represents pressed
 */

uint8_t keyboard_key_state(uint8_t _key_code);
uint8_t _keyboard_register_key_state(struct key_event_t _key_event);

/*
 *  key_event_to_ascii
 *
 *  Converts the content of a key_event to
 *  an ascii character if possible, or returns 0
 *  if non-applicable. Release events are treated
 *  ad non-applicable
 */

char key_event_to_ascii(struct key_event_t _key_event);

/*
 *  _keyboard_scancode_await
 *
 *  Will attempt to read keyboard
 *  scancodes from the data port once
 *  confirmation that data is present
 *  is found on the status port
 *
 *  Returns the scancode
 */


uint8_t _keyboard_scancode_await();

/*
 * _scan_code_to_key_event
 *
 * Will return a key event given a scancode
 * If the scan code does not match it will
 * return the keycode NONE, which is falsy
 *
 * Currently only supports two scan codes
 */

enum key_code_t;

struct key_event_t {
    uint8_t key_code;
    uint8_t flags;
};

struct key_event_t _scan_code_to_key_event(uint8_t* _scan_codes);

/*
 *  key_code_t Enum
 */

enum key_code_t
{
    NONE = 0x00,

    A   = 0x01,
    B   = 0x02,
    C   = 0x03,
    D   = 0x04,
    E   = 0x05,
    F   = 0x06,
    G   = 0x07,
    H   = 0x08,
    I   = 0x09,
    J   = 0x0a,
    K   = 0x0b,
    L   = 0x0c,
    M   = 0x0d,
    N   = 0x0e,
    O   = 0x0f,
    P   = 0x10,
    Q   = 0x11,
    R   = 0x12,
    S   = 0x13,
    T   = 0x14,
    U   = 0x15,
    V   = 0x16,
    W   = 0x17,
    X   = 0x18,
    Y   = 0x19,
    Z   = 0x1a,

    D0  = 0x1b,
    D1  = 0x1c,
    D2  = 0x1d,
    D3  = 0x1e,
    D4  = 0x1f,
    D5  = 0x20,
    D6  = 0x21,
    D7  = 0x22,
    D8  = 0x23,
    D9  = 0x24,

    EXCLAMATION     = 0x30,
    SINGLEQUOTE     = 0x31,
    BACKTICK        = 0x32,
    PERIOD          = 0x33,
    AMP             = 0x34,
    SLASH           = 0x35,
    EQUAL           = 0x36,
    QUESTIONMARK    = 0x37,
    AT              = 0x38,
    POUND           = 0x39,
    DOLLAR          = 0x3a,
    BACKSLASH       = 0x3b,
    PARAGRAPH       = 0x3c,
    TAB             = 0x3d,
    CAPSLOCK        = 0x3e,
    SCROLLOCK       = 0x3f,
    NUMLOCK         = 0x40,
    MINUS           = 0x41,
    ENTER           = 0x42,
    SEMICOLON       = 0x43,
    COMMA           = 0x44,
    SPACE           = 0x45,

    RIGHT_BRACKET       = 0x50,
    LEFT_BRACKET        = 0x51,
    RIGHT_BRACE         = 0x52,
    LEFT_BRACE          = 0x53,
    RIGHT_PARENTHESIS   = 0x54,
    LEFT_PARENTHESIS    = 0x55,

    BACKSPACE   = 0x60,
    DELETE      = 0x61,
    INSERT      = 0x62,
    HOME        = 0x63,
    PAGE_DOWN   = 0x64,
    PAGE_UP     = 0x65,
    ESC         = 0x66,
    END         = 0x67,

    SHIFT_LEFT  = 0x70,
    SHIFT_RIGHT = 0x71,
    ALT_LEFT    = 0x72,
    ALT_RIGHT   = 0x73,
    CTRL_LEFT   = 0x74,
    CTRL_RIGHT  = 0x75,

    UP      = 0x76,
    DOWN    = 0x77,
    LEFT    = 0x78,
    RIGHT   = 0x79,

    KP0     = 0x80,
    KP1     = 0x81,
    KP2     = 0x82,
    KP3     = 0x83,
    KP4     = 0x84,
    KP5     = 0x85,
    KP6     = 0x86,
    KP7     = 0x87,
    KP8     = 0x88,
    KP9     = 0x89,
    KP_ADD  = 0x8a,
    KP_SUB  = 0x8b,
    KP_MUL  = 0x8c,
    KP_DIV  = 0x8d,
    KP_ENTER= 0x8e,

    F1  = 0xa0,
    F2  = 0xa1,
    F3  = 0xa2,
    F4  = 0xa3,
    F5  = 0xa4,
    F6  = 0xa5,
    F7  = 0xa6,
    F8  = 0xa7,
    F9  = 0xa8,
    F10 = 0xa9,
    F11 = 0xaa,
    F12 = 0xab,
};
