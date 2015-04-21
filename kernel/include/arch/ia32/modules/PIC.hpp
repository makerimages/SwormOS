#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>

#define PIC_MASTER          0x20
#define PIC_SLAVE           0xA0
#define PIC_COMMAND         0x00
#define PIC_DATA            0x01
#define PIC_CMD_ENDINTR     0x20
#define PIC_ICW1_ICW4       0x01 // ICW4 (not) needed
#define PIC_ICW1_SINGLE     0x02 // Single (cascade) mode
#define PIC_ICW1_INTERVAL4  0x04 // Call address interval 4 (8)
#define PIC_ICW1_LEVEL      0x08 // Level triggered (edge) mode
#define PIC_CMD_INIT        0x10
#define PIC_MODE_8086       0x01 // 8086/88 (MCS-80/85) mode
#define PIC_MODE_AUTO       0x02 // Auto (normal) EOI
#define PIC_MODE_BUF_SLAVE  0x08 // Buffered mode/slave
#define PIC_MODE_BUF_MASTER 0x0C // Buffered mode/master
#define PIC_MODE_SFNM       0x10 // Special fully nested (not)
#define PIC_READ_IRR        0x0A
#define PIC_READ_ISR        0x0B


uint16_t pic_read_irr();
uint16_t pic_read_isr();
void pic_eoi_slave();
void pic_eoi_master();
void pic_initialize();

#endif
