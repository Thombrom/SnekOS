#include "idt.h"

/*void interrupt_handler(struct cpu_state _cpu, struct stack_state _stack, uint32_t _interrupt)
{
    uint8_t* hex_format = "0123456789abcdef";
    uint8_t* string_format = "Interrupt: 0x00";

    *(string_format + 13) = hex_format[ (_interrupt & 0x000000FF) >> 4 ];
    *(string_format + 14) = hex_format[ (_interrupt & 0x0000000F) ];

    screen_set_cursor_position(0, 0);
    screen_print_string((const char*)string_format);
}

void _idt_load_entry(uint32_t _interrupt)
{
    struct IDT_entry* entry = IDT + _interrupt;

    void* start = interrupt_handler_start;


    entry->offset_high
}*/

void idt_load(void)
{
    for (uint64_t interrupt = 0; interrupt < 256; interrupt++)
    {
        _idt[interrupt].zero = 0;
        _idt[interrupt].offset_low  = (uint16_t)((uint64_t)&isr1 & 0x00000000000FFFF);
        _idt[interrupt].offset_mid  = (uint16_t)(((uint64_t)&isr1 & 0x00000000FFFF0000) >> 16);
        _idt[interrupt].offset_high = (uint32_t)(((uint64_t)&isr1 & 0xFFFFFFFF00000000) >> 32);
        _idt[interrupt].ist         = 0;
        _idt[interrupt].selector    = 0x08;
        _idt[interrupt].types_attr  = 0x8e;
    }

    out_byte(0x21, 0xfd);
    out_byte(0xa1, 0xff);

    idt_load_asm();
}

void isr1_handler(void) {
    uint8_t scan_code;
    in_byte(0x60, &scan_code);

    uint8_t digits[3] = { 0, 0, 0 };
    uint8_t* scan_code_str = "000";

    digits[0] = (scan_code - scan_code % 100) / 100;
    digits[1] = (scan_code - digits[0] * 100 - scan_code % 10) / 10;
    digits[2] = scan_code % 10;

    scan_code_str[0] = 48 + digits[0];
    scan_code_str[1] = 48 + digits[1];
    scan_code_str[2] = 48 + digits[2];

    screen_set_cursor_position(0, 0);
    screen_print_string(scan_code_str);

    out_byte(0x20, 0x20);
    out_byte(0xa0, 0x20);
}
