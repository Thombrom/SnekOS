#include "pic.h"

void pic_acknowledge(uint8_t _interrupt)
{
    // If handled by slave, acknowledge this
    if (_interrupt >= PIC_SLAVE_START_INTERRUPT && _interrupt <= PIC_SLAVE_END_INTERRUPT)
        out_byte(PIC_SLAVE_COMMAND_PORT, PIC_ACKNOWLEDGE);

    out_byte(PIC_MASTER_COMMAND_PORT, PIC_ACKNOWLEDGE);
}

void pic_enable(uint8_t _vector)
{
    uint8_t offset   = _vector > 7 ? _vector - 8 : _vector;
    uint8_t data_port = _vector > 7 ? PIC_SLAVE_DATA_PORT : PIC_MASTER_DATA_PORT;

    // Read current configuration and change position
    uint8_t flags;
    in_byte(data_port, &flags);
    out_byte(data_port, flags | (1 << offset));
}

void pic_disable(uint8_t _vector)
{
    uint8_t offset   = _vector > 7 ? _vector - 8 : _vector;
    uint8_t data_port = _vector > 7 ? PIC_SLAVE_DATA_PORT : PIC_MASTER_DATA_PORT;

    // Read current configuration and change position
    uint8_t flags;
    in_byte(data_port, &flags);
    out_byte(data_port, flags & ~(1 << offset));
}
