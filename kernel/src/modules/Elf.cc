#include <modules/Elf.hpp>
#include <KernelGlobals.hpp>

#include <string.h>

elf_t elf;

void elfInit(multiboot_elf_section_header_table_t* header) {
	elf_sectionHeader_t* sh = reinterpret_cast<elf_sectionHeader_t*>(header-> addr);

	uint32_t shstrtab = sh[header->shndx].addr;

	for(uint32_t i = 0; i < header->num; i++) {
		const char* name = reinterpret_cast<const char*>(shstrtab + sh[i].name);
		if(!strcmp(name,".strtab")) {
			elf.strtab = reinterpret_cast<const char*> (sh[i].addr);
			elf.strtabsz = sh[i].size;
		} else if(!strcmp(name,".symtab")) {
			elf.symtab = reinterpret_cast<elf_symbol_t*> (sh[i].addr);
			elf.symtabsz = sh[i].size;
		}
	}
}

const char* elfLookupSymbol(uint32_t addr) {
	for(uint32_t i = 0; i< (elf.symtabsz/sizeof(elf_symbol_t)); i++) {
		if(ELF32_ST_TYPE(elf.symtab[i].info) != 0x2) {
			continue;
		}
		if((addr >= elf.symtab[i].value) && (addr < (elf.symtab[i].value + elf.symtab[i].size))) {
			return reinterpret_cast<const char*>(reinterpret_cast<uint32_t>(elf.strtab) + elf.symtab[i].name);
		} 

	}
	return NULL;

}



void elfPrintStackTrace()  {
	tm.kputs("Stacktrace follows:\n");
	uint32_t *ebp, *eip;

	__asm__ volatile ("mov %%ebp, %0":"=r"(ebp));
	eip = ebp+1;
	ebp = reinterpret_cast<uint32_t*> (*ebp);
	while (ebp) {
		eip = ebp +1;
		tm.setCursorPos((tm.width/2)-strlen("Stacktrace follows:\n")/2,tm.row);
		tm.kputsf("[0x%x] %s \n",*eip,elfLookupSymbol(*eip));
		ebp = reinterpret_cast<uint32_t*> (*ebp);
	}
}
