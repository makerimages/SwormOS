#include <stdint.h>

#include "libc/IOAccess.hpp"
#include "OSZin/modules/PIC.hpp"
#include "OSZin/kernel/KernelGlobals.hpp"



const uint16_t PIC_MASTER = 0x20;
const uint16_t PIC_SLAVE = 0xA0;
const uint16_t PIC_COMMAND = 0x00;
const uint16_t PIC_DATA = 0x01;
const uint8_t PIC_CMD_ENDINTR = 0x20;
const uint8_t PIC_ICW1_ICW4 = 0x01; // ICW4 (not) needed
const uint8_t PIC_ICW1_SINGLE = 0x02; // Single (cascade) mode
const uint8_t PIC_ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
const uint8_t PIC_ICW1_LEVEL = 0x08; // Level triggered (edge) mode
const uint8_t PIC_CMD_INIT = 0x10;
const uint8_t PIC_MODE_8086 = 0x01; // 8086/88 (MCS-80/85) mode
const uint8_t PIC_MODE_AUTO = 0x02; // Auto (normal) EOI
const uint8_t PIC_MODE_BUF_SLAVE = 0x08; // Buffered mode/slave
const uint8_t PIC_MODE_BUF_MASTER = 0x0C; // Buffered mode/master
const uint8_t PIC_MODE_SFNM = 0x10; // Special fully nested (not)
const uint8_t PIC_READ_IRR = 0x0A;
const uint8_t PIC_READ_ISR = 0x0B;

uint16_t pic_read_irr()
{
    outb(PIC_MASTER + PIC_COMMAND, PIC_READ_IRR);
    outb(PIC_SLAVE + PIC_COMMAND, PIC_READ_IRR);
    return inb(PIC_MASTER + PIC_COMMAND) << 0 |
    inb(PIC_SLAVE  + PIC_COMMAND) << 8;
}

uint16_t pic_read_isr()
{
    outb(PIC_MASTER + PIC_COMMAND, PIC_READ_ISR);
    outb(PIC_SLAVE + PIC_COMMAND, PIC_READ_ISR);
    return inb(PIC_MASTER + PIC_COMMAND) << 0 |
    inb(PIC_SLAVE  + PIC_COMMAND) << 8;
}

void pic_eoi_master()
{
    outb(PIC_MASTER, PIC_CMD_ENDINTR);
 

}

void pic_eoi_slave()
{
    outb(PIC_SLAVE, PIC_CMD_ENDINTR);
}

void pic_initialize()
{
    uint8_t map_irsq_at = 32;
    uint8_t master_mask = 0;
    uint8_t slave_mask = 0;
    outb(PIC_MASTER + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb(PIC_SLAVE + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb(PIC_MASTER + PIC_DATA, map_irsq_at + 0);
    outb(PIC_SLAVE + PIC_DATA, map_irsq_at + 8);
    outb(PIC_MASTER + PIC_DATA, 0x04); // Slave PIC at IRQ2
    outb(PIC_SLAVE + PIC_DATA, 0x02); // Cascade Identity
    outb(PIC_MASTER + PIC_DATA, PIC_MODE_8086);
    outb(PIC_SLAVE + PIC_DATA, PIC_MODE_8086);
    outb(PIC_MASTER + PIC_DATA, master_mask);
    outb(PIC_SLAVE + PIC_DATA, slave_mask);
    
}