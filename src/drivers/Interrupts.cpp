#include "include/Interrupts.h"

Interrupts::Interrupts() {
}

void Interrupts::init(Terminal* term) {
	terminal = term;
	terminal->print("Interrupts enabled: ");
	terminal->print(interruptsEnabled());
	terminal->print(".\n");

	terminal->print("APIC present: ");
	terminal->print(cpuHasAPIC());
	terminal->print(".\n");

	if(!cpuHasAPIC()) {
		terminal->fatalError("No APIC present");
	} else {

		terminal->print("Starting PIC shutdown in favor of APIC... ");
		disablePic();
		terminal->setColor(terminal->makeColor(terminal->COLOR_GREEN, terminal->COLOR_BLACK));
		terminal->print("Done");
		terminal->setColor(terminal->makeColor(terminal->COLOR_LIGHT_GREY, terminal->COLOR_BLACK));
		terminal->print(".\n");

		terminal->print("Starting APIC... ");

		cpuSetAPICBase(cpuGetAPICBase());
		writeRegister(0xF0,readRegister(0xF0));
		__asm__ ("sti");

		terminal->setColor(terminal->makeColor(terminal->COLOR_GREEN, terminal->COLOR_BLACK));
		terminal->print("OK");
		terminal->setColor(terminal->makeColor(terminal->COLOR_LIGHT_GREY, terminal->COLOR_BLACK));
		terminal->print(".\n");

		terminal->print("Interrupts enabled: ");
		terminal->print(interruptsEnabled());
		terminal->print(".\n");
	}
}
void Interrupts::writeRegister(unsigned char reg, unsigned char value)
{
    outb(0x70, reg);
    outb(0x71, value);
}

unsigned char Interrupts::readRegister(unsigned char reg)
{
    outb(0x70, reg);
    return inb(0x71);
}

bool Interrupts::cpuHasAPIC() {
	uint32_t eax, edx;
	cpuid(1, &eax, &edx);
	return edx & 0x200;
}

void Interrupts::disablePic()
{
	outb(0x20, 0x11);
	outb(0xa0, 0x11);

	outb(0x21, 0xe0);
	outb(0xa1, 0xe8);

	outb(0x21, 4);
	outb(0xa1, 2);

	outb(0x21, 1);
	outb(0xa1, 1);

	outb(0x21, 0xff);
	outb(0xa1, 0xff);
}

/* Set the physical address for local APIC registers */
void Interrupts::cpuSetAPICBase(uintptr_t apic)
{
	uint32_t edx = 0;
	uint32_t eax = (apic & 0xfffff100) | IA32_APIC_BASE_MSR_ENABLE;

	#ifdef __PHYSICAL_MEMORY_EXTENSION__
		edx = (apic >> 32) & 0x0f;
	#endif

	cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

uintptr_t Interrupts::cpuGetAPICBase()
{
	uint32_t eax, edx;
	cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);

	#ifdef __PHYSICAL_MEMORY_EXTENSION__
		return (eax & 0xfffff100) | ((edx & 0x0f) << 32);
	#else
		return (eax & 0xfffff100);
	#endif
}