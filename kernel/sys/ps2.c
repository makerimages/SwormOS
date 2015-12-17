#include <ps2.h>
#include <acpi.h>
#include <textmode.h>
#include <io.h>

void ps2_init() {
    failedOnce = false;
    struct FADT* fadt = (struct FADT*) find_table("FACP");
    kprintf("ACPI Revision: %d.\n",fadt->h.Revision);

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

    //Get confbyte
    outb(0x64,0x20);
    ps2_wait();
    uint8_t confbyte = inb(0x60);

    //Edit confbyte
    confbyte &= ~(1<<0);
    confbyte &= ~(1<<1);
    confbyte &= ~(1<<6);
    if(confbyte & (1 << 5)) {
        isDual = true;
        kputs("Dual channel PS/2 controller.\n");
    } else {
        isDual = false;

        kputs("Single channel PS/2 controller.\n");
    }

    outb(0x64,0x60);
    outb(0x60,confbyte);

    //Self-test
    outb(0x64,0xAA);
    ps2_wait();
    uint8_t result = inb(0x60);
    if(result == 0x55) {
        kputcolor(green,black);
        kputs("PS/2 passed self-test.\n");
        kputcolor(lightGrey,black);
        kputs("Performing interface tests...\n");
        outb(0x64,0xAB);
        ps2_wait();
        result = inb(0x60);
        if(result == 0x00) {
            kputcolor(green,black);
            kputs("Port 1 pass.\n");
            kputcolor(lightGrey,black);
            port1OK = true;

        } else {
            kputcolor(red,black);
            kputs("Port 1 fail!\n");
            kputcolor(lightGrey,black);
            port1OK = false;
        }
        if(isDual) {
            outb(0x64,0xA9);
            ps2_wait();
            result = inb(0x60);
            if(result == 0x00) {
                kputcolor(green,black);
                kputs("Port 2 pass.\n");
                kputcolor(lightGrey,black);
                port2OK = true;
            } else {
                kputcolor(red,black);
                kputs("Port 2 fail!\n");
                kputcolor(lightGrey,black);
                port2OK = false;
            }
        } else {
            kputs("Skipping Port 2.\n");
            port2OK = false;
        }
        kputs("Enabling PS/2 devices.\n");
        if(port1OK) {
            outb(0x64,0xAE);
            kputs("Device on port 1 enabled.\n");

        }
        if (port2OK) {
            outb(0x64,0xA8);
            kputs("Device on port 2 enabled.\n");
        }
        //Get confbyte
        outb(0x64,0x20);
        ps2_wait();
        confbyte = inb(0x60);

        //Edit confbyte
        confbyte |= 1<<0;
        if(port2OK) {
            confbyte |= 1<<1;
        }
        outb(0x64,0x60);
        outb(0x60,confbyte);
        ps2_wait_input();
        outb(0x60,0xFF);
        ps2_wait();
        result = inb(0x60);
        if(result == 0xFA) {
            kputcolor(green,black);
            kputs("Device 1 reset success.\n");
            kputcolor(lightGrey,black);
        }
        if(port2OK) {
            outb(0x64,0xD4);
            ps2_wait_input();
            outb(0x60,0xFF);
            ps2_wait();
            result = inb(0x60);
            if(result == 0xAA) {
                kputcolor(green,black);
                kputs("Device 2 reset success.\n");
                kputcolor(lightGrey,black);
            } else {
                kputcolor(red,black);
                kputs("Device 2 reset fail.\n");
                kputcolor(lightGrey,black);
            }

        }
    } else {
        kputcolor(red,black);
        kputs("PS/2 failed self-test! Trying setup once more...\n");
        kputcolor(lightGrey,black);
        if(!failedOnce) {
            ps2_setup();
        } else {
            kpanic("Failed to set up PS/2 for the second time. Abort.");
        }
    }
}

void ps2_wait() {
    while(inb(0x64) & 2);
}

void ps2_wait_input() {
    while(!(inb(0x64) & 4));

}
