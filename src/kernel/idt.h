#pragma once

#include "typedef.h"
#include "io.h"
#include "../driver/screen_vga.h"

/*
 *  Interrupt description table
 *
 *  Code for dealing with setting up the IDT
 *  symbol _idt declared in linker script
*/

struct IDT_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

extern struct IDT_entry _idt[256];

/*
 *  Offset is a pointer to code in the segment
 *  Segment selector is the offset in the GDT
 *  Flags:
 *  (15)        - Present in memory
 *  (14 13)     - Privilige (who can call)
 *  (12)        - 0
 *  (11)        - Size of gate (32/16)
 *  (10 9 8)    - 1 1 0
 *  (7 6 5)     - 0 0 0
 *  (4 3 2 1 0) - Reserved
 */


/*
 * Interrupt handler called from
 * the IDT. CPU_state is set to the
 * values of the registers at the
 * interrupt, and stack_state represents
 * the stack at the time  of interrupt.
 */

/*struct cpu_state {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} __attribute__((packed));

struct stack_state {
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed));

void interrupt_handler(struct cpu_state _cpu, struct stack_state _stack, uint32_t _interrupt);
*/
/*
 *  Loads the IDT
 *
 */

void idt_load(void);
extern  uint64_t isr1;
extern void idt_load_asm(void);

