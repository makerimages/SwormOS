#ifndef HEAP_H_
#define HEAP_H_
#define BITS(s, name) unsigned int name : s
#include <multiboot.h>
#include <stdint.h>
#include <stdbool.h>


#define KHEAP_START 0xC000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE 0x20000
#define HEAP_MAGIC 0x123890AB
#define HEAP_MIN_SIZE 0x7000

typedef struct heap_header {
	struct heap_header * prev;
	struct heap_header * next;
	BITS(1, allocated);
	BITS(31, length);
} heap_header_t;

typedef struct heap {
	heap_header_t * first;
	uint32_t startAddress;
	uint32_t endAddress;
	uint32_t maxAddress;
	bool rw;
	bool user;
} heap_t;


heap_t * heap_create(uint32_t start, uint32_t end, uint32_t max, bool rw, bool user);

void * alloc(uint32_t size, bool pageAlign, heap_t * heap);
void free(void *p, heap_t *heap);

void * kmalloc_int(uint32_t size, bool align, uint32_t * phys);
void * kmalloc(uint32_t size);
void * kmalloc_a(uint32_t size);
void * kmalloc_p(uint32_t size, uint32_t * phys);
void * kmalloc_ap(uint32_t size, uint32_t * phys);

void kfree(void * p);
static void chunk_invalidate(heap_header_t* chunk) ;

#endif
