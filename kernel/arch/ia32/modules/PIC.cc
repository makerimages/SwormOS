#include <arch/ia32/modules/PIC.hpp>
#include <KernelGlobals.hpp>
#include <ioports.hpp>

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
    uint8_t master_mask = 0xFF;
    uint8_t slave_mask = 0xFF;
    outb (PIC_MASTER + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb (PIC_SLAVE + PIC_COMMAND, PIC_CMD_INIT | PIC_ICW1_ICW4);
    outb (PIC_MASTER + PIC_DATA, map_irsq_at + 0);
    outb (PIC_SLAVE + PIC_DATA, map_irsq_at + 8);
    outb (PIC_MASTER + PIC_DATA, 0x04); // Slave PIC at IRQ2
    outb (PIC_SLAVE + PIC_DATA, 0x02); // Cascade Identity
    outb (PIC_MASTER + PIC_DATA, PIC_MODE_8086);
    outb (PIC_SLAVE + PIC_DATA, PIC_MODE_8086);
    outb (PIC_MASTER + PIC_DATA, master_mask);
    outb (PIC_SLAVE + PIC_DATA, slave_mask);
    pic_eoi_master ();
    pic_eoi_slave ();
}
