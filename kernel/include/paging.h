#ifndef PAGING_H_
#define PAGING_H_
#define BITS(s, name) unsigned int name : s
#include <stdbool.h>
#include <stdint.h>


typedef struct paging_page {
	BITS( 1, present);
	BITS( 1, rw);
	BITS( 1, user);
	BITS( 1, accessed);
	BITS( 1, dirty);
	BITS( 7, unused);
	BITS(20, frame);
} paging_page_t;

typedef struct paging_table {
	paging_page_t pages[1024];
} paging_table_t;

typedef struct paging_directory {
	paging_table_t * tables[1024];

	void * tablesPhysical[1024];

	void * physicalAddress;
} paging_directory_t;

void paging_init();
void paging_switchDirectory(paging_directory_t * directory);
paging_page_t * paging_getPage(uint32_t address, bool make, paging_directory_t * directory);

void frameAlloc(paging_page_t * page, bool rw, bool user);

paging_directory_t * paging_cloneDirectory(paging_directory_t * directory);

#endif
