#include <paging.h>
#include <heap.h>
#include <multiboot.h>
#include <string.h>
#include <idt.h>
#include <stdbool.h>
#include <stdint.h>

enum {
	PAGE_PRESENT = 0x1,
	PAGE_WRITE = 0x2,
	PAGE_USER = 0x4
};

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void pageFault(registers_t * regs);
static void frameSet(uint32_t address);
//static void frameClear(uint32_t address);
//static bool frameTest(uint32_t address);
static uint32_t frameFirst();
//static void frameFree(paging_page_t * page);
static paging_table_t * cloneTable(paging_table_t * talbe, uint32_t * phys);
void copyPagePhysical(uint32_t p1, uint32_t p2);

extern void * placementAddress;
extern void * kernelHeap;

paging_directory_t * kernelDirectory = NULL;
paging_directory_t * currentDirectory = NULL;

uint32_t * frames;
uint32_t frameCount;

void paging_init(multiboot_info_t * multiboot) {
	uint32_t memorySize = (multiboot->mem_lower + multiboot->mem_upper);

	frameCount = memorySize / 4;
	frames = kmalloc(INDEX_FROM_BIT(frameCount));
	memset(frames, 0, INDEX_FROM_BIT(frameCount));

	kernelDirectory = currentDirectory = (paging_directory_t *)kmalloc_a(sizeof(paging_directory_t));

	uint32_t i = 0;
	while (i < (uint32_t)placementAddress + 0x1000) {
		frameAlloc(paging_getPage(i, 1, kernelDirectory), false, true);
		i += 0x1000;
	}

	for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i+= 0x1000)
		frameAlloc(paging_getPage(i, 1, kernelDirectory), false, true);


	paging_switchDirectory(kernelDirectory);
	uint32_t cr0;
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ volatile("mov %0, %%cr3" :: "r"(cr0));
	kernelHeap = heap_create(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);
}

void paging_switchDirectory(paging_directory_t * directory) {
	currentDirectory = directory;
	__asm__ volatile("mov %0, %%cr3" :: "r"(directory->tablesPhysical));

}


paging_page_t * paging_getPage(uint32_t address, bool make, paging_directory_t * directory) {
	address /= 0x1000;
	uint32_t tableIDX = address / 1024;
	if (directory->tables[tableIDX])
		return &directory->tables[tableIDX]->pages[address%1024];
	else if (make) {
		uint32_t tmp;
		directory->tables[tableIDX] = (paging_table_t *)kmalloc_ap(sizeof(paging_table_t), &tmp);
		directory->tablesPhysical[tableIDX] = (void *)(tmp | (PAGE_PRESENT | PAGE_WRITE | PAGE_USER));
		return &directory->tables[tableIDX]->pages[address%1024];
	} else
		return NULL;
}


static void frameSet(uint32_t address) {
	uint32_t frame = address / 0x1000;
	frames[INDEX_FROM_BIT(frame)] |= (0x1 << OFFSET_FROM_BIT(frame));
}

/*static void frameClear(uint32_t address) {
	uint32_t frame = address / 0x1000;
	frames[INDEX_FROM_BIT(frame)] &= ~(0x1 << OFFSET_FROM_BIT(frame));
}

static bool frameTest(uint32_t address) {
	uint32_t frame = address / 0x1000;
	return !!(frames[INDEX_FROM_BIT(frame)] & (0x1 << OFFSET_FROM_BIT(frame)));
}*/

static uint32_t frameFirst() {
	for (uint32_t i = 0; i < INDEX_FROM_BIT(frameCount); i++) {
		if (frames[i] == 0xFFFFFFFF)
			continue;
		for (uint32_t j = 0; j < 32; j++) {
			if (!(frames[i] & (0x1 << j)))
				return i * 4 * 8 + j;
		}
	}
	return 0xFFFFFFFF;
}

void frameAlloc(paging_page_t * page, bool rw, bool user) {
	if (!page->frame)
		return;
	uint32_t idx = frameFirst();
	if (idx == 0xFFFFFFFF)
		kpanic("No free frames!");
	frameSet(idx * 0x1000);
	page->present = 1;
	page->rw = rw;
	page->user = user;
	page->frame = idx;
}

/*static void frameFree(paging_page_t * page) {
	uint32_t frame;
	if (!(frame = page->frame))
		return;
	frameClear(frame);
	page->frame = 0x0;
}*/


static paging_table_t * cloneTable(paging_table_t * src, uint32_t * phys) {
	paging_table_t * table = (paging_table_t *)kmalloc_ap(sizeof(paging_table_t), phys);
	memset(table, 0, sizeof(paging_directory_t));

	for(int i = 0; i < 1024; i++) {
		if (!src->pages[i].frame)
			continue;

		frameAlloc(&table->pages[i], false, true);

		src->pages[i].present  = table->pages[i].present;
		src->pages[i].rw       = table->pages[i].rw;
		src->pages[i].user     = table->pages[i].user;
		src->pages[i].accessed = table->pages[i].accessed;
		src->pages[i].dirty    = table->pages[i].dirty;

		copyPagePhysical(src->pages[i].frame * 0x1000, table->pages[i].frame * 0x1000);
	}
	return table;
}

paging_directory_t * paging_cloneDirectory(paging_directory_t * src) {
	uint32_t phys;
	paging_directory_t * dir = (paging_directory_t *)kmalloc_ap(sizeof(paging_directory_t), &phys);

	memset(dir, 0, sizeof(paging_directory_t));

	uint32_t offset = (uint32_t) dir->tablesPhysical - (uint32_t) dir;

	dir->physicalAddress = (void *)(phys + offset);

	for (int i = 0; i < 1024; i++) {
		if (!src->tables[i])
			continue;

		if (kernelDirectory->tables[i] == src->tables[i]) {
			dir->tables[i] = src->tables[i];
			dir->tablesPhysical[i] = src->tablesPhysical[i];
		} else {
			dir->tables[i] = cloneTable(src->tables[i], &phys);
			dir->tablesPhysical[i] = (void *)(phys | 0x07);
		}
	}
	return dir;
}
