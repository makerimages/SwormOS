#include "../drivers/include/PIC.h"
#include "../../include/libc/String.h"
#include "../drivers/include/Multiboot.h"
#include "../drivers/include/InterruptDescriptorTable.h"
#include "../../include/kernel/KernelGlobals.h"
/* Hardware text mode color constants. */

Terminal terminal;

char buffer[33];
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(multiboot_info* mbt, unsigned int magic)

{
	terminal.Init();
	terminal.print("OS Zin v 0.0.1.\n");
	terminal.print("Copyright (c) Makerimages 2014-2015.\n");
	terminal.print("Magic number is: ");
	terminal.print(uitoa(magic,buffer,16));
	terminal.print(".\n");

	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbt->mmap_addr;
	while(mmap <(multiboot_memory_map_t*)  mbt->mmap_addr + mbt->mmap_length) {
		mmap = (multiboot_memory_map_t*) ( (uintptr_t) mmap + mmap->size + sizeof(unsigned int) );
	}

	terminal.print("Memory size: ");
	terminal.print(uitoa(mbt->mem_upper + mbt->mem_lower,buffer,10));
	terminal.print("KB\n");

	initIDT();
	pic_initialize();
	__asm__("sti");
	terminal.setColor(terminal.makeColor(terminal.COLOR_GREEN,terminal.COLOR_BLACK));
	terminal.print("Interrupts initialized");
	terminal.fatalError("OK?");

}
