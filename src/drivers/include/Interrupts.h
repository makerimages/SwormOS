#ifndef INTERRUPT_H
#define INTERRUPT_H

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#include "Terminal.h"
#include "System.h"

class Interrupt {
	public:
		Interrupt();
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

		void disablePic();
		void cpuSetAPICBase(uintptr_t apic);
		uintptr_t cpuGetAPICBase();
		Terminal* terminal;
};

#endif