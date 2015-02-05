#include <stdint.h>
#include "modules/PIC.hpp"

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
	uint8_t irq = int_ctx->int_no - 32;

	// Handle the potentially spurious interrupts IRQ 7 and IRQ 15.
	if ( irq == 7 && !(pic_read_isr() & (1 << 7)) ) {
		return;
	}

	if ( irq == 15 && !(pic_read_isr() & (1 << 15)) ) {
		return pic_eoi_master();

	}

	(void) int_ctx;

			// Send the end of interrupt signal to the relevant PICs.
	if ( 8 <= irq ) {
		pic_eoi_slave();
		pic_eoi_master();	
	}
	
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void interrupt_handler(struct interrupt_context* int_ctx)
{
	if ( int_ctx->int_no < 32 )
		isr_handler(int_ctx);
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no < 32 + 16 )
		irq_handler(int_ctx);
}
