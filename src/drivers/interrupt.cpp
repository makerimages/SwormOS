#include "include/interrupt.h"

Interrupt::Interrupt() {
}

void Interrupt::init(Terminal* term) {
	terminal = term;
	terminal -> print("Interrupts enabled: ");
	terminal -> print(are_interrupts_enabled());
	terminal -> print(".\n");
	terminal -> print("APIC present: ");
	terminal -> print(cpuHasAPIC());
	terminal -> print(".\n");
	terminal -> print("Starting PIC shutdown in favor of APIC\n");
	terminal -> print("Masking all lines on the PIC... ");
	for(int i = 0 ; i <= 15; i++) {
		IRQ_set_mask(i);
	}
	terminal -> setColor(terminal -> make_color(terminal -> COLOR_GREEN, terminal -> COLOR_BLACK));
	terminal -> print("OK");
	terminal -> setColor(terminal -> make_color(terminal -> COLOR_LIGHT_GREY, terminal -> COLOR_BLACK));
	terminal -> print(".\n");

}

bool Interrupt::cpuHasAPIC() {
	uint32_t eax, edx;
	cpuid(1, &eax, &edx);
	return edx & 0x200;
}

void Interrupt::IRQ_set_mask(unsigned char IRQline) {
	uint16_t port;
	uint8_t value;

	if(IRQline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		IRQline -= 8;
	}
	value = inb(port) | (1 << IRQline);
	outb(port, value);        
}