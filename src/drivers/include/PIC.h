#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>
#include "../../../include/libc/IOAccess.h"

uint16_t pic_read_irr();
uint16_t pic_read_isr();
void pic_eoi_slave();
void pic_eoi_master();
void pic_initialize();

#endif
