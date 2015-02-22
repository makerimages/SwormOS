#ifndef ELF_H
#define ELF_H

#include <stdint.h>
#include "OSZin/kernel/Multiboot.h"

#define ELF32_ST_TYPE(i) ((i) & 0xF)

typedef struct {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
} __attribute__ ((packed)) elf_sectionHeader_t;

typedef struct {
	uint32_t name;
	uint32_t value;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
} __attribute__ ((packed)) elf_symbol_t;

typedef struct {
	elf_symbol_t* symtab;
	uint32_t symtabsz;
	const char* strtab;
	uint32_t strtabsz;
} elf_t;

void elf_init(multiboot_elf_section_header_table_t* header);
const char* elf_lookupSymbol(uint32_t addr);

void elf_printStackTrace();

#endif