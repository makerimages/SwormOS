#ifndef INTERRUPT_H
#define INTERRUPT_H

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

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
		void IRQ_set_mask(unsigned char IRQline);
		Terminal* terminal;
};

#endif