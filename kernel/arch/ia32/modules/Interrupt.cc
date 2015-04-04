#include <arch/ia32/modules/PIC.hpp>
#include <arch/ia32/modules/IDT.hpp>
#include <KernelGlobals.hpp>
#include <ioports.hpp>

#include <stdint.h>

struct interrupt_context
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, userss;
};

void isr_handler(struct interrupt_context* int_ctx)
{
	(void) int_ctx;
	uint8_t isr = int_ctx->int_no - 32;

}

void irq_handler(struct interrupt_context* int_ctx)
{
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
		pic_eoi_slave();
		pic_eoi_master();
}

extern "C" //C extern
void interrupt_handler(struct interrupt_context* int_ctx)
{
	tm.kputs("HANDLER\n");

	if ( int_ctx->int_no < 32 ) {
		tm.kputs("heh\n");
		isr_handler(int_ctx);
	}
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no <= 32 + 16 ) {
		tm.kputs("heh\n");
		irq_handler(int_ctx);
		pic_eoi_master();
	}
}
