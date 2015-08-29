#include <pic.h>
#include <idt.h>
#include <textmode.h>
#include <io.h>

#include <stdint.h>



void isr_handler(interrupt_context* int_ctx) {
	uint8_t irq = int_ctx->int_no;
  if(isrHandlers[irq]) {
    isrHandlers[irq](int_ctx);
  }
  else {
  	kpanic("Unhandled ISR!");
  }

  	pic_eoi_master();
  	pic_eoi_slave();
}

void irq_handler(interrupt_context* int_ctx) {
 	uint8_t irq = int_ctx->int_no - 32;
	if ( irq == 7 && !(pic_read_isr() & (1 << 7)) ) {
		return;

	}
	if ( irq == 15 && !(pic_read_isr() & (1 << 15)) ) {
		return;
		pic_eoi_master();
	}

  if(irqHandlers[irq]) {
    irqHandlers[irq](int_ctx);
  }
  else {
    kpanic("Unhandled IRQ!");
  }

	pic_eoi_master();
	pic_eoi_slave();
}
void interrupt_handler(interrupt_context* int_ctx) {

	if ( int_ctx->int_no < 32 ) {
		isr_handler(int_ctx);
	}
	else if ( 32 <= int_ctx->int_no && int_ctx->int_no <= 32 + 16 ) {

		irq_handler(int_ctx);
		pic_eoi_master();
	}
}
