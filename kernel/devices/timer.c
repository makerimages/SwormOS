#include <timer.h>
#include <pic.h>
#include <textmode.h>
#include <io.h>
#include <stdint.h>

static void timer_callback(interrupt_context * regs) {
	ticks++;
}

void init_timer(uint32_t freq) {
	kprintf("Initing timer");
    ticks = 0;
    int divisor = PIT_FREQUENCY / freq;
   	outb (PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
    uint32_t l = (uint8_t)(divisor & 0xFF);
    uint32_t h = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_COUNTER0, l);
    outb(PIT_COUNTER0, h);

	set_handler(1,0,&timer_callback);
    /* Unmask the IRQ0. */
    outb (PIC_MASTER + PIC_DATA, 0xFE);

}
