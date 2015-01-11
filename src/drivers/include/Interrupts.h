#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "Terminal.h"
#include "System.h"

class Interrupts {
	public:
		Interrupts();
		void init(Terminal * term);
	protected:
		bool cpuHasAPIC();
		inline bool interruptsEnabled() {
			unsigned long flags;
			asm volatile ( "pushf\n\t"
				"pop %0"
				: "=g"(flags) );
			return flags & (1 << 9);
		}
		Terminal* terminal;
};

#endif
