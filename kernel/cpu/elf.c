#include <elf.h>
#include <textmode.h>

#include <string.h>

elf_t elf;

void elf_init(multiboot_elf_section_header_table_t* header) {
	elf_sectionHeader_t* sh = (elf_sectionHeader_t*)(header-> addr);

	uint32_t shstrtab = sh[header->shndx].addr;

	for(uint32_t i = 0; i < header->num; i++) {
		const char* name = (const char*) (shstrtab + sh[i].name);
		if(!strcmp(name,".strtab")) {
			elf.strtab = (const char*) sh[i].addr;
			elf.strtabsz = sh[i].size;
		} else if(!strcmp(name,".symtab")) {
			elf.symtab = (elf_symbol_t*) sh[i].addr;
			elf.symtabsz = sh[i].size;
		}
	}
}

const char* elf_lookup_symbol(uint32_t addr) {
	for(uint32_t i = 0; i< (elf.symtabsz/sizeof(elf_symbol_t)); i++) {
		if(ELF32_ST_TYPE(elf.symtab[i].info) != 0x2) {
			continue;
		}
		if((addr >= elf.symtab[i].value) && (addr < (elf.symtab[i].value + elf.symtab[i].size))) {
			return (const char*) (uint32_t)elf.strtab + elf.symtab[i].name;
		}

	}
	return NULL;

}



void elf_trace()  {
	kputs("Stacktrace follows:\n");
	uint32_t *ebp, *eip;

	__asm__ __volatile__ ("mov %%ebp, %0":"=r"(ebp));
	eip = ebp+1;
	ebp = (uint32_t*) *ebp;
	while (ebp) {
		eip = ebp +1;
		set_pos((width/2)-strlen("Stacktrace follows:\n")/2,row);
		kprintf("[0x%x] %s \n",*eip,elf_lookup_symbol(*eip));
		ebp = (uint32_t*)*ebp;
	}
}
