#pragma once

#include "../kernel/io.h"
#include "../kernel/typedef.h"

/*
 * All the serial ports are calculated relative to the data port
 * and the ports (COM1, COM2, ...) have ports in the same order but
 * starting at different values
 */

#define SERIAL_COM1_BASE                0x3F8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/*
 *  SERIAL_COMMAND
 *  Dataport is 16 bits, but we can only send one byte at a time,
 *  this tells that we will send first the 8 highest and afterwards
 *  the 8 lowest bits
 */

#define SERIAL_LINE_ENABLE_DLAB         0x80

/*
 *  CONFIGURATION
 */
#define SERIAL_PORT_WRITE_TRIES         100

/*
 *  serial_configure_baud_rate
 *  This will set the speed (in baud) of the data being sent over
 *  the _com serial port. _divisor means the speed will be
 *  (115200 / _divisor) bits/ss
 *
 *  @param _com         The COM port to use
 *  @param _divisor     Baud rate divisor
 */

void serial_configure_baud_rate(uint16_t _com, uint16_t _divisor);

/*
 *  serial_configure_line
 *  This is made up of flags. The following flags are available
 *  (7)     - enable/disable DLAB
 *  (6)     - break control
 *  (5 4 3) - number of parity bits
 *  (2)     - number of stop bits
 *  (1 0)   - Data length
 *
 *  For now standard config of 0x03 is used. To be changed in the 
 *  future for more flexibilty
 *
 *  @param _com     COM port
 */

void serial_configure_line(uint16_t _com);

/*
 *  serial_configure_buffers
 *  This is made up of flags.  The f ollowing flags are available
 *  (7 6)   - How many bytes for FIFO buffers
 *  (5)     - 16/64 byte buffers
 *  (4)     - Reserved
 *  (3)     - How should port data be accessed
 *  (2)     - Clear transmission FIFO buffer
 *  (1)     - Clear reciever FIFO buffer
 *  (0)     - Enable FIFO buffer
 *
 *  For now standard config of 0xC7 = 11000111 is used. To be chaned
 *  in the future for more flexibility
 *
 *  @param _com     COM port
 */

void serial_configure_buffers(uint16_t _com);

/*
 *  serial_configure_modem
 *  This is made up of flags. The following flags are available
 *  (7)     - Reserved
 *  (6)     - Reserved
 *  (5)     - Autoflow enabled
 *  (4)     - Loopback for debug
 *  (3)     - Auxillary output 2 for interrupts
 *  (2)     - Auxillary output 1
 *  (1)     - Ready to transmit
 *  (0)     - Data terminal ready
 *
 *  For now standard config of 0x03. To be changed in the future for more
 *  flexibility
 *
 *  @param _com     COM port
 */

void serial_configure_modem(uint16_t _com);

/*
 *  serial_is_transmit_fifo_empty
 *  Check whether the transmit buffer is emptry
 *
 *  @param _com     COM port
 *  @return         0 if nonempty, otherwise 1
 */

uint8_t serial_is_transmit_fifo_empty(uint16_t _com);

/*
 *  serial_write
 *  
 *  Write to serial port
 */

uint8_t serial_write(uint16_t _com, const char* _str);


