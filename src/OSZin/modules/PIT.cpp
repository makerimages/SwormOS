#include "OSZin/modules/PIT.hpp"
#include "libc/IOAccess.hpp"
#include "OSZin/kernel/KernelGlobals.hpp"
Pit::Pit() {

}

void Pit::init(int freq) {
	tm.kputsf("Trying to initialize PIT to %d Hz.\n",freq);
	int divisor = PIT_FREQUENCY / freq;
   	outb(PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    outb(PIT_COUNTER0, divisor );
    outb(PIT_COUNTER0, divisor >> 8);
    tm.kputs("Initialization routine ran. \n");
}

