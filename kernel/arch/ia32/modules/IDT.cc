#include <arch/ia32/modules/IDT.hpp>
#include <arch/ia32/modules/PIC.hpp>
#include <KernelGlobals.hpp>
#include <ioports.hpp>

#include <string.h>
#include <stdint.h>
#include <KernelGlobals.hpp>


#define IDT_TYPE_INTERRUPT 0xE /* Interrupts disabled in eflags. */
#define IDT_TYPE_TRAP 0xF      /* Interrupts unaffected in eflags. */

#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_DPL_SHIFT 5
#define IDT_FLAG_DPL_BITS 2
#define IDT_FLAG_TYPE_SHIFT 0
#define IDT_FLAG_TYPE_BITS 4


idt_interruptHandler_t isrHandlers[32];
idt_interruptHandler_t irqHandlers[16];

struct idt_entry
{
	uint16_t handler_low;
	uint16_t selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t handler_high;
} __attribute__((packed));

struct idt_entry idt[256];

void lidt(uintptr_t base, size_t limit)
{
	asm volatile ("subl $6, %%esp\n\t"
	              "movw %w0, 0(%%esp)\n\t"
	              "movl %1, 2(%%esp)\n\t"
	              "lidt (%%esp)\n\t"
	              "addl $6, %%esp" : : "rN"(limit), "r"(base));
}

void idt_format_normal_entry(struct idt_entry* entry,
                             void (*handler)(void),
                             uint8_t type,
                             uint8_t rpl)
{
	entry->handler_low = (uintptr_t) handler >> 0 & 0xFFFF;
	entry->selector = 0x08 /* Kernel Code Segment */;
	entry->ist = 0;
	entry->flags = IDT_FLAG_PRESENT |
	               rpl << IDT_FLAG_DPL_SHIFT |
	               type << IDT_FLAG_TYPE_SHIFT;
	entry->handler_high = (uintptr_t) handler >> 16 & 0xFFFF;
}

static void isr0CallBack(interrupt_context * regs) {
	tm.panic("Divide error");
}
static void isr1CallBack(interrupt_context * regs) {
	tm.panic("Debug");
}
static void isr3CallBack(interrupt_context * regs) {
	tm.panic("Breakpoint");
}
static void isr4CallBack(interrupt_context * regs) {
	tm.panic("Overflow");
}
static void isr5CallBack(interrupt_context * regs) {
	tm.panic("Bound range exeeded");
}
static void isr6CallBack(interrupt_context * regs) {
	tm.panic("Invalid opcode");
}
static void isr7CallBack(interrupt_context * regs) {
	tm.panic("No math coprocessor");
}
static void isr8CallBack(interrupt_context * regs) {
	tm.panic("Double fault");
}
static void isr9CallBack(interrupt_context * regs) {
	tm.panic("Coprocessor segment overrun");
}
static void isr10CallBack(interrupt_context * regs) {
	tm.panic("Invalid TSS");
}
static void isr11CallBack(interrupt_context * regs) {
	tm.panic("Segment not present");
}
static void isr12CallBack(interrupt_context * regs) {
	tm.panic("Stack-segment fault");
}
static void isr13CallBack(interrupt_context * regs) {
	tm.panic("GP");
}
static void isr14CallBack(interrupt_context * regs) {
	tm.panic("Page fault");
}
static void isr16CallBack(interrupt_context * regs) {
	tm.panic("Floating point error");
}
static void isr17CallBack(interrupt_context * regs) {
	tm.panic("Alignment check");
}
static void isr18CallBack(interrupt_context * regs) {
	tm.panic("Machine check");
}
static void isr19CallBack(interrupt_context * regs) {
	tm.panic("Streaming SIMD Extensions");
}

void idt_initialize()
{
	memset(&idt, 0, sizeof(idt));
	idt_format_normal_entry(&idt[0], isr0, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[1], isr1, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[2], isr2, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[3], isr3, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[4], isr4, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[5], isr5, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[6], isr6, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[7], isr7, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[8], isr8, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[9], isr9, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[10], isr10, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[11], isr11, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[12], isr12, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[13], isr13, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[14], isr14, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[15], isr15, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[16], isr16, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[17], isr17, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[18], isr18, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[19], isr19, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[20], isr20, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[21], isr21, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[22], isr22, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[23], isr23, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[24], isr24, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[25], isr25, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[26], isr26, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[27], isr27, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[28], isr28, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[29], isr29, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[30], isr30, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[31], isr31, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[32], irq0, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[32], irq0, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[33], irq1, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[34], irq2, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[35], irq3, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[36], irq4, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[37], irq5, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[38], irq6, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[39], irq7, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[40], irq8, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[41], irq9, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[42], irq10, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[43], irq11, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[44], irq12, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[45], irq13, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[46], irq14, IDT_TYPE_INTERRUPT, 0x0);
	idt_format_normal_entry(&idt[47], irq15, IDT_TYPE_INTERRUPT, 0x0);
	lidt((uintptr_t) idt, sizeof(idt) - 1);
	setHandler(0,0,&isr0CallBack);
	setHandler(0,1,&isr1CallBack);
	setHandler(0,3,&isr3CallBack);
	setHandler(0,4,&isr4CallBack);
	setHandler(0,5,&isr5CallBack);
	setHandler(0,6,&isr6CallBack);
	setHandler(0,7,&isr7CallBack);
	setHandler(0,8,&isr8CallBack);
	setHandler(0,9,&isr9CallBack);
	setHandler(0,10,&isr10CallBack);
	setHandler(0,11,&isr11CallBack);
	setHandler(0,12,&isr12CallBack);
	setHandler(0,13,&isr13CallBack);
	setHandler(0,14,&isr14CallBack);
	setHandler(0,16,&isr16CallBack);
	setHandler(0,17,&isr17CallBack);
	setHandler(0,18,&isr18CallBack);
	setHandler(0,19,&isr19CallBack);
}

void setHandler(int type,int index, idt_interruptHandler_t handler) {
	switch(type) {
		case 0:
			isrHandlers[index]= handler;
			break;
		case 1:
			irqHandlers[index]= handler;
			break;

		default:
			isrHandlers[index]= handler;

			break;
	}
}
