#include "OSZin/kernel/Multiboot.h"
#include "OSZin/modules/TextMode.hpp"

TextMode tm;
uint32_t totalMem = 0;
uint32_t usableMem = 0;
uintptr_t initial_esp;
#define RAM 1

extern "C" //Use C linkage
void kernelMain(multiboot_info* mbt ,unsigned int magic, uintptr_t esp) {
	initial_esp = esp;
	tm.init();
	tm.kputs("Booting OSZin version 0.0.1\n");
	tm.kputs("Copyright (C) Makerimages 2014-2015. Licenced under the MIT licence.\n");
	tm.kputs("Visit www.oszin.cf for more information. Sources available on GitHub.\n");
	tm.setColor(tm.lightBlue,tm.black);
	tm.kputs("System details follow:\n");
	tm.resetColor();
	tm.kputsf("\tStarted with ESP:0x%x.\n",&initial_esp);
	tm.kputsf("\tMagic number is: 0x%x. Not 0x2badb002 ? I don't know how you got here. \n",magic);
	tm.kputsf("\tMultiboot info points to:0x%x\n",&mbt);
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
}