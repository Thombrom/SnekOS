#pragma once

#include "typedef.h"
#include "io.h"

#define PS2_DATA_PORT      0x60
#define PS2_STATUS_PORT    0x64
#define PS2_COMMAND_PORT   0x64

/*
 *  Various funcitons for communicating
 *  with the PS/2 controller in the system
 */

uint8_t ps2_outbuffer_full();
uint8_t ps2_inbuffer_full();
uint8_t ps2_timeout_error();
uint8_t ps2_parity_error();

uint8_t ps2_read_data();
