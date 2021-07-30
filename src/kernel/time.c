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

    pic_enable(PIC_MASTER_START_INTERRUPT);
}

void time_interrupt_handler(struct interrupt_frame  _frame)
{
    time_since_boot += 1;
}

uint64_t time_get_time_since_boot()
{
    return time_since_boot;
}
