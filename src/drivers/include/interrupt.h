#ifndef INTERRUPT_H
#define INTERRUPT_H

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#include "terminal.h"
#include "system.h"

class Interrupt {
	public:
		Interrupt();
		void init(Terminal * term);
	protected:
		bool cpuHasAPIC();
		inline bool are_interrupts_enabled()
		{
			unsigned long flags;
			asm volatile ( "pushf\n\t"
				"pop %0"
				: "=g"(flags) );
			return flags & (1 << 9);
		}
		void disable_pic();
		void cpuSetAPICBase(uintptr_t apic);
		uintptr_t cpuGetAPICBase();
		Terminal* terminal;
};

#endif