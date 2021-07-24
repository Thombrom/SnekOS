#include "idt.h"

/*
 *  Declarations only relevant for this
 *  compilation unit:
 *      - isr_stub_table:
 *          Table with ISR handler locations in memory
 *      - idtr:
 *          Interrupt descriptor table with size and locatio
 *          of descriptors
 */

extern void* isr_stub_table[];
struct idtr_t idtr;

/*
 *  Implementation
 */

void idt_set_descriptor(uint8_t _vector, void* _isr, uint8_t _flags)
{
    struct idt_entry_t* descriptor = &_idt[_vector];

    descriptor->isr_low         = (uint64_t)_isr % 0xFFFF;
    descriptor->kernel_cs       = 0x08;
    descriptor->ist             = 0;
    descriptor->attributes      = _flags;
    descriptor->isr_mid         = ((uint64_t)_isr >> 16) & 0xFFFF;
    descriptor->isr_high        = ((uint64_t)_isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved        = 0;

}


void idt_init() {
    // Set up the IDTR values
    idtr.base = (uint64_t)&_idt[0];
    idtr.limit = 4095;

    // Populate descriptor table
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    // Load idt (lidt) and enable interrupts (sti)
    __asm__ volatile ("lidt %0" :  : "memory"(idtr));
    out_byte(0x21, 0xfd);           // Mask PIC1 to only keyboard interrupts
    out_byte(0xa1, 0xff);           // Mask PIC2 to no interrupts
    __asm__ volatile ("sti");
}

void interrupt_handler(struct interrupt_frame _frame)
{
    screen_set_cursor_position(0, 0);
    uint8_t scan_code;
    in_byte(0x60, &scan_code);

    //scan_code = (uint8_t)_frame.interrupt;

    uint8_t digits[3] = { 0, 0, 0 };
    uint8_t* scan_code_str = "000";

    digits[0] = (scan_code - scan_code % 100) / 100;
    digits[1] = (scan_code - digits[0] * 100 - scan_code % 10) / 10;
    digits[2] = scan_code % 10;

    scan_code_str[0] = 48 + digits[0];
    scan_code_str[1] = 48 + digits[1];
    scan_code_str[2] = 48 + digits[2];

    screen_print_string(scan_code_str);

    out_byte(0x20, 0x20);
    out_byte(0xa0, 0x20);

}
