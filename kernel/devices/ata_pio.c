#include "ata_pio.h"
#include "textmode.h"
#include <io.h>

void ata_identify() {
    kprintf("Identifying drives...\n");
    ata_identify_primary();
/*    //Select Master Disk
    outb(0x176,0xA0);

    outb(0x172,0x00);
    outb(0x173,0x00);
    outb(0x174,0x00);
    outb(0x175,0x00);

    //Identi7y

    outb(0x177,0xEC);

        kprintf("0x%b \n",inb(0x177));
        kprintf("0x%x and 0x%x\n",inb(0x174),inb(0x175));
        while(inb(0x177) & 0x80 )
        {
            kprintf("1");
        }
        kprintf("Cleared\n");
        kprintf("0x%b\n",inb(0x177));
        */

}

void ata_identify_primary() {

        //Select Master Disk
        outb(0x1F6,0xA0);

        outb(0x1F2,0x00);
        outb(0x1F3,0x00);
        outb(0x1F4,0x00);
        outb(0x1F5,0x00);

        //Identify

        outb(0x1F7,0xEC);

        // No disk
        if(inb(0x1F7) == 0x00) {
            kputcolor(darkGrey,black);
            kprintf("Primary - Master = NO DISK\n");
            kputcolor(lightGrey,black);
        } else {
            kputcolor(darkGrey,black);
            kprintf("Primary - Master = DISK, ATA");
            // Wait for BSY to clear
            while(inb(0x1F7) & 0x80 )  {

            }
            // ATAPI detect
            if(inb(0x1F4) != 0x00 || inb(0x1F4) != 0x00 ) {
                kprintf("PI\n");
            } else {
                kprintf("\n");
            }
            kputcolor(lightGrey,black);
            while(!(inb(0x1F7) & 1 << 3)) {

            }
            // ERR clean
            if(!(inb(0x1F7) & 0x01)) {
                //TODO: Parse device data somehow
            }

        }


}
