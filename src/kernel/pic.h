#pragma once

#include "typedef.h"
#include "io.h"

#define PIC_MASTER_COMMAND_PORT     0x20
#define PIC_MASTER_DATA_PORT        0x21
#define PIC_SLAVE_COMMAND_PORT      0xa0
#define PIC_SLAVE_DATA_OPRT         0xa1

#define PIC_MASTER_START_INTERRUPT  0x08
#define PIC_MASTER_END_INTERRUPT    PIC_MASTER_START_INTERRUPT + 7
#define PIC_SLAVE_START_INTERRUPT   0x70
#define PIC_SLAVE_END_INTERRUPT     PIC_SLAVE_START_INTERRUPT + 7

#define PIC_ACKNOWLEDGE             0x20

void pic_acknowledge(uint8_t _interrupt);
