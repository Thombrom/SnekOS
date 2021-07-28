#include "time.h"

/*
 *  Variables applicable to this
 *  compilation unit
 */

uint64_t time_since_boot;

/*
 * Implementaiton
 */

void time_init()
{
    time_since_boot = 0;

    uint8_t interrupt_vector = PIC_MASTER_START_INTERRUPT + 0;
    register_interrupt_handler(&time_interrupt_handler, interrupt_vector);
}

void time_interrupt_handler(struct interrupt_frame  _frame)
{
    screen_set_cursor_position(0, 0);
    time_since_boot += 1;

    uint8_t digits[8];

    for (uint8_t itt = 0;  itt < 8; itt++)
        digits[itt] = (time_since_boot >> (4 * itt)) & 0x0F;

    char* lookup_table = "0123456789ABCDEF";
    for (uint8_t itt = 7; itt < 8; itt--)
        screen_putchar(lookup_table[digits[itt]]);

}
