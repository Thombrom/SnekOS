#include "ps2.h"

uint8_t ps2_outbuffer_full()
{
    uint8_t ps2_status;
    in_byte(PS2_STATUS_PORT, &ps2_status);

    return ps2_status & 1;
}

uint8_t ps2_inbuffer_full()
{
    uint8_t ps2_status;
    in_byte(PS2_STATUS_PORT, &ps2_status);

    return ps2_status & (1 << 1);
}

uint8_t ps2_timeout_error()
{
    uint8_t ps2_status;
    in_byte(PS2_STATUS_PORT, &ps2_status);

    return ps2_status & (1 << 6);
}

uint8_t ps2_parity_error()
{
    uint8_t ps2_status;
    in_byte(PS2_STATUS_PORT, &ps2_status);

    return ps2_status & (1 << 7);
}

uint8_t ps2_read_data()
{
    uint8_t ps2_data;
    in_byte(PS2_DATA_PORT, &ps2_data);

    return ps2_data;
}
