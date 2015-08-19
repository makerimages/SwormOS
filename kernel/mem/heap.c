#include <heap.h>
#include <paging.h>
#include <stdbool.h>
#include <textmode.h>

extern void * end;
extern paging_directory_t * kernelDirectory;

void * placementAddress = &end;
heap_t * kernelHeap = NULL;

static void expand(heap_t * heap, uint32_t newSize);
static void chunk_split(heap_header_t * chunk, uint32_t len);
static void chunk_glue(heap_header_t * chunk);

heap_t * heap_create(uint32_t start, uint32_t end, uint32_t max, bool rw, bool user) {
	heap_t * heap = (heap_t *) kmalloc(sizeof(heap_t));
	if (start % 0x1000)
		return NULL;
	if (end % 0x1000)
		return NULL;

	heap->startAddress = start;
	heap->endAddress = start; //end;
	heap->maxAddress = max;
	heap->rw = rw;
	heap->user = user;

	expand(heap, end - start);

	heap_header_t * hole = (heap_header_t *) start;
	hole->prev = hole->next = NULL;
	hole->allocated = false;
	hole->length = end - start;

	heap->first = hole;
	return heap;
}

static void expand(heap_t * heap, uint32_t newSize) {
	if (newSize & 0xFFFFF000) {
		newSize &= 0xFFFFF000;
		newSize += 0x1000;
	}

	if (heap->startAddress + newSize > heap->maxAddress) {
		kpanic("Out of heap space");
	}

	uint32_t oldSize = heap->endAddress - heap->startAddress;
	for (uint32_t i = oldSize; i < newSize; i += 0x1000)
		frameAlloc(paging_getPage(heap->startAddress + i, 1, kernelDirectory), heap->rw, heap->user);

	heap->endAddress = heap->startAddress+newSize;
}

void * alloc(uint32_t size, bool pageAlign, heap_t * heap) {
	heap_header_t * cur = heap->first;
	heap_header_t * prev = cur;
	if (!cur)
		kpanic("Invalid heap!");

	while (cur) {
		if (!cur->allocated && cur->length >= size) {
			chunk_split(cur, size);
			cur->allocated = 1;
			return (void *)((uint32_t)cur + sizeof(heap_header_t));
		}
		prev = cur;
		cur = cur->next;
	}

	uint32_t loc = (uint32_t)prev + prev->length;
	if (pageAlign && (loc+sizeof(heap_header_t)) & 0xFFFFFF00) {
		loc &= 0xFFFFFF00;
		loc += 0x100 - sizeof(heap_header_t);
	}
	if (loc + sizeof(heap_header_t) + size > heap->endAddress) {
		uint32_t oldLength = heap->endAddress - heap->startAddress;
		expand(heap, oldLength + size + sizeof(heap_header_t));
	}

	heap_header_t * newChunk = (heap_header_t *)loc;
	newChunk->prev = prev;
	newChunk->next = NULL;
	newChunk->allocated = true;
	newChunk->length = size;

	prev->next = newChunk;
	return (void *)((uint32_t)newChunk + sizeof(heap_header_t));
}

void free(void *p, heap_t * heap) {
	if (!p) {
		return;
	}
	if ((uint32_t)p < heap->startAddress || heap->endAddress < (uint32_t)p) {
		kpanic("Trying to free from the wrong heap");
	}
	heap_header_t * header = (heap_header_t *)((uint32_t)p - sizeof(heap_header_t));
	header->allocated = false;
	chunk_glue(header);
}

void * kmalloc_int(uint32_t size, bool align, uint32_t * phys) {
	if (kernelHeap) {
		void * addr = alloc(size, align, kernelHeap);
		if (phys) {
			paging_page_t * page = paging_getPage((uint32_t)addr, 0, kernelDirectory);
			*phys = (uint32_t)page->frame * 0x100 + ((uint32_t)addr & 0xFFF);
		}
		return addr;
	} else {
		if (align && (uint32_t)placementAddress & 0xFFFFF000) {
			placementAddress = (void *)((uint32_t)placementAddress & 0xFFFFF000);
			placementAddress = (void *)((uint32_t)placementAddress + 0x1000);
		}
		if (phys)
			*phys = (uint32_t)placementAddress;

		void * tmp = placementAddress;
		placementAddress = (void *)((uint32_t)placementAddress + size);
		return tmp;
	}
}
void * kmalloc(uint32_t size) {
	return kmalloc_int(size, false, NULL);
}
void * kmalloc_a(uint32_t size) {
	return kmalloc_int(size, true, NULL);
}
void * kmalloc_p(uint32_t size, uint32_t * phys) {
	return kmalloc_int(size, false, phys);
}
void * kmalloc_ap(uint32_t size, uint32_t * phys) {
	return kmalloc_int(size, true, phys);
}

void kfree(void * ptr) {
	free(ptr, kernelHeap);
}

static void chunk_split(heap_header_t * chunk, uint32_t len) {
	heap_header_t * newChunk = (heap_header_t *)((uint32_t)chunk + len + sizeof(heap_header_t));
	newChunk->prev = chunk;
	newChunk->next = chunk->next;
	newChunk->allocated = false;
	newChunk->length = chunk->length - len - sizeof(heap_header_t);

	chunk->next = newChunk;
	chunk->length = len;
}

//Chunk is not valid after you call this!
static void chunk_glue(heap_header_t * chunk) {
	while(chunk -> prev && !chunk->prev->allocated) {kputs("while 1\n");chunk = chunk->prev;}
	while (chunk->next && !chunk->next->allocated) {
		kputs("while2\n");
		chunk->length = chunk->length + chunk->next->length + sizeof(heap_header_t);
		if (chunk->next->next) {
			kputs("if in while\n");
			chunk->next->next->prev = chunk;
		}
		chunk->next = 0xDEADBEAF;
		chunk->next = chunk->next->next;
	}

}
