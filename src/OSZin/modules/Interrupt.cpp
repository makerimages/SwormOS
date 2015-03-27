#include <stdint.h>
#include "libc/IOAccess.hpp"
#include "OSZin/modules/PIC.hpp"
#include "OSZin/modules/IDT.hpp"
#include "OSZin/kernel/KernelGlobals.hpp"
struct interrupt_context
{
	uint32_t cr2;
	uint32_t gs;
	uint32_t fs;
	uint32_t ds;
	uint32_t es;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp; /* If (cs & 0x3) != 0 */
	uint32_t ss;  /* If (cs & 0x3) != 0 */
};

void isr_handler(struct interrupt_context* int_ctx)
{
	(void) int_ctx;
}

void irq_handler(struct interrupt_context* int_ctx)
{
	tm.kputs("Seeing which irq handler to use\n");
	switch(int_ctx->int_no) {
		case 33:
			irq0();
		default:
			isr6();

	}
	    pic_eoi_master();
	
}
extern "C" //C extern
void interrupt_handler(struct interrupt_context* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		isr_handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16 )
		irq_handler(int_ctx);

}