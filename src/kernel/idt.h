#pragma once

#include "typedef.h"
#include "io.h"
#include "pic.h"

#include "../driver/screen_vga.h"

/*
 *  idt_entry_t
 *
 *  Interrupt Descriptor Table Entry
 *   - ISR:
 *      Memory location of interrupt handler
 *   - kernel_cs:
 *      Code segment for kernel. Defined in GDT
 *      (see GDT.asm)
 *   - attributes:
 *      Flags for various settings
 */

struct idt_entry_t {
    uint16_t    isr_low;
    uint16_t    kernel_cs;
    uint8_t     ist;
    uint8_t     attributes;
    uint16_t    isr_mid;
    uint32_t    isr_high;
    uint32_t    reserved;
} __attribute__((packed));

/*
 *  idtr_t
 *  Interrut Descriptor Table
 *   - limit:
 *      Size of table take one
 *   - base:
 *      Pointer to first entry
 */

struct idtr_t {
    uint16_t    limit;
    uint64_t    base;
} __attribute__((packed));

/*
 *  interrupt_frame
 *
 *  A representation of the frame generated
 *  upon interrupts.
 *
 *   - interrupt:
 *      Interrupt number (0 - 31 mapped)
 *   - error_code:
 *      For interrupts with error codes
 *   - rip:
 *      Original stack pointer before interrupt
 *   - rflags
 *      Flags regsiter
 *   - cs
 *      Code segment - defined in GDT
 *   - RIP
 *      Instruction pointer
 *
 *  Note: The location of these on the stack have
 *  not been tested properly, so they might need to
 *  be reordered if used
 */

struct interrupt_frame {
    uint64_t interrupt;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
} __attribute__((packed));

/*
 *  _idt
 *  Actual IDT entry table. Symbol defined
 *  in linker script
 */

extern struct idt_entry_t _idt[256];

/*
 *  idt_init
 *
 *  Initialize the IDT and map various assembly
 *  routines to the interrupt. These routines
 *  then call the common interrupt handler defined
 *  in C and located further down
 */

void idt_init(void);

/*
 *  idt_set_descriptor
 *
 *  Helper function used to load the location
 *  of an interrupt service routine to a position
 *  in the IDTR and set flags
 */

void idt_set_descriptor(uint8_t _vector, void* _isr, uint8_t _flags);

/*
 *  Common interrupt handler. This is pointed to by
 *  all interrupt handlers
 */

void interrupt_handler(struct interrupt_frame _frame);


/*
 *  Regisetering interrupt handlers
 *
 *  Interrupt handler type and registry for the handlers.
 *  Function to register, taking a function pointer and an
 *  interrupt it will be registered to
 */

typedef void (*interrupt_handler_t)(struct interrupt_frame _frame);

void register_interrupt_handler(interrupt_handler_t _handler, uint8_t _interrupt);
