#include <arch/ia32/modules/PIT.hpp>
#include <arch/ia32/modules/PIC.hpp>
#include <KernelGlobals.hpp>
#include <ioports.hpp>

Pit::Pit() {

}

void Pit::init(int freq) {
	tm.kputsf ("Trying to initialize PIT to %d Hz.\n",freq);
	int divisor = PIT_FREQUENCY / freq;
   	outb (PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    outb (PIT_COUNTER0, divisor );
    outb (PIT_COUNTER0, divisor >> 8);

    /* Unmask the IRQ0. */
    outb (PIC_MASTER + PIC_DATA, 0xFE);

    tm.kputs ("Initialization routine ran. \n");
}
