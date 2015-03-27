#include "OSZin/modules/PIT.hpp"
#include "libc/IOAccess.hpp"
Pit::Pit() {

}

void Pit::init(int freq) {
	int divisor = PIT_FREQUENCY / freq;
   	outb(PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    outb(PIT_COUNTER0, divisor);
    outb(PIT_COUNTER0, divisor >> 8);
}