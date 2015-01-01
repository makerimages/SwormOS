#include "drivers/include/terminal.h"
#include "drivers/include/interrupt.h"
#include "drivers/include/system.h"
#include "drivers/include/multiboot.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */

Terminal terminal;
Interrupt interrupt;


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
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}
	terminal.print("Memory size: ");
	terminal.print(uitoa(mbt->mem_upper + mbt->mem_lower,buffer,10));
	terminal.print("KB\n");

	interrupt.init(&terminal);
}
