#include <pmm_new.h>
#include <textmode.h>
#include <stdint.h>
void pmm_new_init(uintptr_t start_addr,uint32_t length_bytes) {
    kprintf(" length %d\n",length_bytes);
    uint32_t frames;
    uintptr_t start = ROUND_TO_PAGES(start_addr);
    uint32_t mem_length_bytes = length_bytes & ~PAGE_SIZE;
    for (uintptr_t addr = start; addr <= start+mem_length_bytes;addr+=PAGE_SIZE) {
        frames++;
    }
    kprintf("%d\n",frames);

};
