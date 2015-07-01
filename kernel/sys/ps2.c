#include <ps2.h>
#include <acpi.h>
#include <textmode.h>
#include <io.h>

void ps2_init() {
    struct FADT* fadt = (struct FADT*) find_table("FACP");
    if(fadt->h.Revision > 1) {
        if(fadt->BootArchitectureFlags & (1 << 1)) {
           kputs("There is a PS/2 controller");
           ps2_setup();
        } else {
           kputcolor(red,black);
           kputs("Unable to find a PS/2 controller. Possible failure of features.\n");
           kputcolor(lightGrey,black);
       }
   } else {
       kputcolor(brown,black);
       kputs("ACPI revision not capable enough to determine 8042. Assuming one exists...\n");
       kputcolor(lightGrey,black);
       ps2_setup();

   }



}

void ps2_setup() {
    kputs("Starting PS/2 initialization\n");
    //Disable devices
    outb(0x64,0xAD);
    ps2_wait();
    outb(0x64,0xA7);
    // Flush buffer
    uint8_t crap = inb(0x60);
    kprintf("0x%x\n",crap);

    //Get confbyte
    outb(0x64,0x20);
    ps2_wait();
    uint8_t confbyte = inb(0x60);
    kprintf("0x%x\n",confbyte);

    //Edit confbyte
    confbyte &= ~(1<<0);
    confbyte &= ~(1<<1);
    confbyte &= ~(1<<6);
    if(confbyte & (1 << 5)) {
        isDual = true;
    } else {
        isDual = false;
    }
    outb(0x64,0x20);
    outb(0x60,confbyte);
    kprintf("0x%x\n",confbyte);

    //Self-test
    outb(0x64,0xAA);
    ps2_wait();
    uint8_t result = inb(0x60);
    kprintf("0x%x\n",result);
}

void ps2_wait() {
    while(inb(0x64) & 2);
}
