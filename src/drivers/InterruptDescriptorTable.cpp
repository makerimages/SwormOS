#include <stdint.h>
#include <stddef.h>

#include "include/InterruptDescriptorTable.h"
#include "include/System.h"

#define IDT_TYPE_INTERRUPT 0xE
#define IDT_TYPE_TRAP 0xF

#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_DPL_SHIFT 5
#define IDT_FLAG_DPL_BITS 2
#define IDT_FLAG_TYPE_SHIFT 0
#define IDT_FLAG_TYPE_BITS 4



#define bit(n) (1<<(n)) /* Set bit n to 1 */
 
/* Check if bit n in flags is set */
#define check_flag(flags, n) ((flags) & bit(n))
struct idtEntry {
	uint16_t handlerLow;
	uint16_t selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t handlerHigh;
};

struct idtEntry idt[256];

void* memset(void* bufptr, int value, size_t size)
{
	unsigned char* buf = (unsigned char*) bufptr;
	for ( size_t i = 0; i < size; i++ )
		buf[i] = (unsigned char) value;
	return bufptr;
}

void lidt(uintptr_t base, size_t limit) {
	asm volatile("subl $6, %%esp\n\t"
				 "movw %w0, 0(%%esp)\n\t"
				 "movl %1, 2(%%esp)\n\t"
				 "lidt (%%esp)\n\t"
				 "addl $6, %%esp" : : "rN"(limit), "r"(base));
}

void idtFormatNormalEntry(struct idtEntry* entry, void (*handler)(void), uint8_t type, uint8_t rpl) {
	entry->handlerLow = (uintptr_t) handler >> 0 & 0xFFFF;
	entry->selector = 0x08;
	entry->ist = 0;
	entry->flags = IDT_FLAG_PRESENT | rpl << IDT_FLAG_DPL_SHIFT | type << IDT_FLAG_TYPE_SHIFT;
	entry->handlerHigh = (uintptr_t) handler >> 16 & 0xFFFF;
}

void initIDT() {
	memset(&idt,0, sizeof(idt));
	
	idtFormatNormalEntry(&idt[0], isr0, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[1], isr1, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[2], isr2, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[3], isr3, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[4], isr4, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[5], isr5, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[6], isr6, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[7], isr7, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[8], isr8, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[9], isr9, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[10], isr10, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[11], isr11, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[12], isr12, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[13], isr13, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[14], isr14, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[15], isr15, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[16], isr16, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[17], isr17, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[18], isr18, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[19], isr19, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[20], isr20, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[21], isr21, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[22], isr22, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[23], isr23, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[24], isr24, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[25], isr25, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[26], isr26, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[27], isr27, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[28], isr28, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[29], isr29, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[30], isr30, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[31], isr31, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[32], irq0, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[33], irq1, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[34], irq2, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[35], irq3, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[36], irq4, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[37], irq5, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[38], irq6, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[39], irq7, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[40], irq8, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[41], irq9, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[42], irq10, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[43], irq11, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[44], irq12, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[45], irq13, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[46], irq14, IDT_TYPE_INTERRUPT, 0x0);
	idtFormatNormalEntry(&idt[47], irq15, IDT_TYPE_INTERRUPT, 0x0);
	lidt((uintptr_t) idt, sizeof(idt)-1);


}

void isr0(){};
void isr1(){    
	Terminal terminal;
	terminal.Init();
	terminal.fatalError("Success");
};
void isr2(){};
void isr3(){};
void isr4(){};
void isr5(){};
void isr6(){};
void isr7(){};
void isr8(){};
void isr9(){};
void isr10(){};
void isr11(){};
void isr12(){};
void isr13(){};
void isr14(){};
void isr15(){};
void isr16(){};
void isr17(){};
void isr18(){};
void isr19(){};
void isr20(){};
void isr21(){};
void isr22(){};
void isr23(){};
void isr24(){};
void isr25(){};
void isr26(){};
void isr27(){};
void isr28(){};
void isr29(){};
void isr30(){};
void isr31(){};

void irq0(){};
void irq1(){Terminal terminal;
	terminal.Init();
	terminal.fatalError("Success");}
void irq2(){};
void irq3(){};
void irq4(){};
void irq5(){};
void irq6(){};
void irq7(){};
void irq8(){};
void irq9(){};
void irq10(){};
void irq11(){};
void irq12(){};
void irq13(){};
void irq14(){};
void irq15(){};