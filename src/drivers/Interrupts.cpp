#include "include/Interrupts.h"
#include "include/PIC.h"

Interrupts::Interrupts() {
}

void Interrupts::init(Terminal* term) {
	terminal = term;
	terminal->print("Interrupts enabled: ");
	terminal->print(interruptsEnabled());
	terminal->print(".\n");

	terminal->print("Doing PIC stuff. ");
	pic_initialize();
	__asm__("sti");
	terminal->setColor(terminal->makeColor(terminal->COLOR_GREEN, terminal->COLOR_BLACK));
	terminal->print("Done");
	terminal->setColor(terminal->makeColor(terminal->COLOR_LIGHT_GREY, terminal->COLOR_BLACK));
	terminal->print(".\n");



	terminal->print("Interrupts enabled: ");
	terminal->print(interruptsEnabled());
	terminal->print(".\n");

}
