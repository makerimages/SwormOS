/* Ports input/output. */

#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

extern "C" {

/* arch/.../ioports.s */
uint8_t inb (uint16_t);
void    outb (uint16_t, uint8_t);
void    outw (uint16_t, uint16_t);

}

#endif
