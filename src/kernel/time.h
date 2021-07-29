#pragma once

#include "idt.h"
#include "../driver/screen_vga.h"

/*
 *  time_init
 *
 *  For time initialization. Should be
 *  run after IDT initialization as it
 *  hooks interrupt handler.
 */

void time_init();

/*
 *  time_interrupt_handler
 *
 *  Interrupt handler for the timing
 *  event. Hooks PIC0 interrupt
 */

void time_interrupt_handler(struct interrupt_frame _frame);

/*
 *  time_get_time_since_boot
 *
 *  Returns the number of interrupts
 *  since boot. This is probably ~16 x seconds
 *  since boot
 */

uint64_t time_get_time_since_boot();
