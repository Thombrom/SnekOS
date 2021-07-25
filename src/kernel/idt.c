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
interrupt_handler_t interrupt_handler_table[32];

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
    out_byte(0x21, 0xfd);                               // Mask PIC1 to only keyboard interrupts
    out_byte(0xa1, 0xff);                               // Mask PIC2 to no interrupts
    __asm__ volatile ("sti");
}

void interrupt_handler(struct interrupt_frame _frame)
{
    uint8_t interrupt = (uint8_t)_frame.interrupt;

    if (interrupt > 31)
    {   // We cannot handle this right now
        pic_acknowledge(interrupt);
        return;
    }

    if (!interrupt_handler_table[interrupt])
    {   // We don't have handler registered
        pic_acknowledge(interrupt);
        return;
    }

    interrupt_handler_table[interrupt](_frame);
    pic_acknowledge(interrupt);
    return;
}

void register_interrupt_handler(interrupt_handler_t _handler, uint8_t _interrupt)
{
    if (_interrupt > 31)
        return;

    interrupt_handler_table[_interrupt] = _handler;
}
