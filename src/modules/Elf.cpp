#include "modules/Elf.hpp"
#include "libc/String.hpp"
#include "kernel/KernelGlobals.hpp"

elf_t kernel_elf;

void elf_init(multiboot_elf_section_header_table_t* header) {
	elf_sectionHeader_t* sh = reinterpret_cast<elf_sectionHeader_t*>(header-> addr);

	uint32_t shstrtab = sh[header->shndx].addr;

	for(uint32_t i = 0; i < header->num; i++) {
		const char* name = reinterpret_cast<const char*>(shstrtab + sh[i].name);
		if(!strcmp(name,".strtab")) {
			kernel_elf.strtab = reinterpret_cast<const char*> (sh[i].addr);
			kernel_elf.strtabsz = sh[i].size;
		} else if(!strcmp(name,".symtab")) {
			kernel_elf.symtab = reinterpret_cast<elf_symbol_t*> (sh[i].addr);
			kernel_elf.symtabsz = sh[i].size;
		}
	}
}

const char* elf_lookupSymbol(uint32_t addr, elf_t* elf) {
	for(uint32_t i = 0; i< (elf -> symtabsz/sizeof(elf_symbol_t)); i++) {
		if(ELF32_ST_TYPE(elf->symtab[i].info) != 0x2) {
			continue;
		}
		if(addr >= elf-> symtab[i].value && addr < (elf -> symtab[i].value + elf -> symtab[i].size)) {
			return reinterpret_cast<const char*>(reinterpret_cast<uint32_t>(elf->strtab + elf->symtab[i].name));
		} 

	}
	return NULL;

}

void elf_printStackTrace()  {
	uint32_t *ebp, *eip;
	char buffer[65];

	__asm__ volatile ("mov %%ebp, %0":"=r"(ebp));
	eip = ebp+1;
	ebp = reinterpret_cast<uint32_t*> (*ebp);
	while (ebp) {
		eip = ebp +1;
		terminal.kprintf(" [0x%x] %s \n",*eip,elf_lookupSymbol(*eip,&kernel_elf));
		ebp = reinterpret_cast<uint32_t*> (*ebp);
	}
}