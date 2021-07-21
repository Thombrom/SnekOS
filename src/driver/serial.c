#include "serial.h"

void serial_configure_baud_rate(uint16_t _com, uint16_t _divisor)
{
    out_byte(SERIAL_LINE_COMMAND_PORT(_com),    SERIAL_LINE_ENABLE_DLAB);
    out_byte(SERIAL_DATA_PORT(_com),            (_divisor >>  8) & 0x00FF);
    out_byte(SERIAL_DATA_PORT(_com),            _divisor & 0x00FF);
}

void serial_configure_line(uint16_t _com)
{
    out_byte(SERIAL_LINE_COMMAND_PORT(_com), 0x03);
}

void serial_configure_buffers(uint16_t _com)
{
    out_byte(SERIAL_FIFO_COMMAND_PORT(_com), 0xC7);
}

void serial_configure_modem(uint16_t _com)
{
    out_byte(SERIAL_MODEM_COMMAND_PORT(_com), 0x03);
}

uint8_t serial_is_transmit_fifo_empty(uint16_t _com)
{
    uint8_t result;
    in_byte(SERIAL_LINE_STATUS_PORT(_com), &result);
    return result;
}

uint8_t serial_write(uint16_t _com, const char* _str)
{
    uint8_t* out_ptr = (uint8_t*)_str;
    uint8_t tries = 0;

    // Attempt to write to FIFO buffer
    while (*out_ptr != '\0') 
    {
        uint8_t fifo_empty = 0;

        while (!fifo_empty && tries++ < SERIAL_PORT_WRITE_TRIES)
            fifo_empty = serial_is_transmit_fifo_empty(_com);

        // We cannot seem to write
        if (tries >= SERIAL_PORT_WRITE_TRIES) return 0;

        out_byte(SERIAL_DATA_PORT(_com), *out_ptr++);
    }

    return 1;
}
