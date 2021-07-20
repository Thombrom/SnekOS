#include "io.h"

void out_byte(unsigned short _port, unsigned char _value)
{
    asm volatile ("outb %0, %1" :  : "a"(_value), "Nd"(_port));
}

void in_byte(unsigned short _port, unsigned char* _return)
{
    asm volatile ("inb %1, %0" : "=a"(*_return) : "Nd"(_port));
}
