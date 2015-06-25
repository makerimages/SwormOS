#include <timer.h>
#include <pic.h>
#include <textmode.h>
#include <io.h>

static void timer_callback(interrupt_context * regs) {
//	ticks++;
    kputs("D");
}

void init_timer(int freq) {
    int divisor = PIT_FREQUENCY / freq;
   	outb (PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    outb (PIT_COUNTER0, divisor );
    outb (PIT_COUNTER0, divisor >> 8);

	set_handler(1,0,&timer_callback);
    /* Unmask the IRQ0. */
    outb (PIC_MASTER + PIC_DATA, 0xFE);

}
