#include "OSZin/kernel/Multiboot.h"
#include "OSZin/kernel/KernelGlobals.hpp"
#include "OSZin/modules/Elf.hpp"
#include "OSZin/modules/IDT.hpp"
#include "OSZin/modules/PIC.hpp"
#include "OSZin/modules/PIT.hpp"
#include "libc/IOAccess.hpp"
//Initialize Kernel Globals
TextMode tm;
uint32_t totalMem;
uint32_t usableMem;
uintptr_t initial_esp;

extern "C" //Use C linkage
void kernelMain(multiboot_info* mbt ,unsigned int magic) {
	elfInit(&mbt -> u.elf_sec);

	tm.init();
	tm.kputs("Booting OSZin version 0.0.1\n");
	tm.kputs("Copyright (C) Makerimages 2014-2015. Licenced under the MIT licence.\n");
	tm.kputs("Visit www.oszin.cf for more information. Sources available on GitHub.\n");
	tm.setColor(tm.lightBlue,tm.black);
	tm.kputs("System details follow:\n");
	tm.resetColor();
	tm.kputsf("\tMagic number is: 0x%x.",magic);
	tm.setColor(tm.lightBlue,tm.black);
	tm.kputsf(" Not 0x2badb002 ? I don't know how you got here. \n");
	tm.resetColor();
	tm.kputsf("\tMultiboot info points to: 0x%x\n",&mbt);
	if(mbt->flags & (1 << 6) ){
		tm.setColor(tm.green,tm.black);
		tm.kputs("\tUsing passed memory map.\n");
		tm.resetColor();
		multiboot_memory_map_t* mmap = reinterpret_cast <multiboot_memory_map_t*> (mbt->mmap_addr);
		while(mmap < reinterpret_cast<multiboot_memory_map_t*>(mbt->mmap_addr + mbt->mmap_length)) {
			mmap = reinterpret_cast <multiboot_memory_map_t*> (reinterpret_cast<uintptr_t> (mmap) + mmap->size + sizeof(unsigned int) );
			totalMem += mmap->len;
			if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE ) {
				usableMem += mmap->len;
			}
		}
   		totalMem = totalMem/1024;
   		usableMem = usableMem/1024;
   		tm.kputsf("\tTotal memory: %d KB.\n",totalMem);
   		tm.kputsf("\tOf which %d KB is usable.\n",usableMem);
	} else if(mbt->flags & (1 << 0)) {
		tm.setColor(tm.red,tm.black);
		tm.kputs("\tNo memory map passed, using mem_upper & mem_lower.\n");
		tm.kputsf("\tTotal memory: %d KB.\n",mbt->mem_upper + mbt->mem_lower);
		tm.resetColor();
	}
	
	tm.setColor(tm.lightBlue,tm.black);
	tm.kputs("System details ended.\n");
	tm.resetColor();
	tm.kputs("Enabling interrupts.\n");
	idt_initialize();
	pic_initialize();
	tm.setColor(tm.green,tm.black);
	tm.kputs("Enabled.\n");
	tm.resetColor();
	Pit pp;
	pp.init(1000);
		__asm__("sti");

}