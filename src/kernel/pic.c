#include "pic.h"

void pic_acknowledge(uint8_t _interrupt)
{
    // If handled by slave, acknowledge this
    if (_interrupt >= PIC_SLAVE_START_INTERRUPT && _interrupt <= PIC_SLAVE_END_INTERRUPT)
        out_byte(PIC_SLAVE_COMMAND_PORT, PIC_ACKNOWLEDGE);

    out_byte(PIC_MASTER_COMMAND_PORT, PIC_ACKNOWLEDGE);
}
