#include <arch/ia32/modules/PIC.hpp>
#include <arch/ia32/modules/IDT.hpp>
#include <KernelGlobals.hpp>
#include <ioports.hpp>

#include <stdint.h>

struct interrupt_context {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, userss;
};

void isr_handler(struct interrupt_context* int_ctx) {
	(void) int_ctx;
	uint8_t irq = int_ctx->int_no - 32;
}

void irq_handler(struct interrupt_context* int_ctx) {
 	uint8_t irq = int_ctx->int_no - 32;
	if ( irq == 7 && !(pic_read_isr() & (1 << 7)) ) {
		return;

	}
	if ( irq == 15 && !(pic_read_isr() & (1 << 15)) ) {
		return;
		pic_eoi_master();
	}
	(void) int_ctx;
	/* TODO: PARSE IRQ's*/
		
	pic_eoi_master();
	pic_eoi_slave();
}
int pitTicks = 0;
extern "C" //C extern
void interrupt_handler(struct interrupt_context* int_ctx) {

	if ( int_ctx->int_no < 32 ) {
		isr_handler(int_ctx);
	}
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no <= 32 + 16 ) {
		pitTicks++;
		if(pitTicks == 100) {
			tm.kputs("h");
			pitTicks = 0;
		}
		irq_handler(int_ctx);
		pic_eoi_master();
	}
}
