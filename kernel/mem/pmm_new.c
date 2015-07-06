#include <pmm_new.h>
#include <textmode.h>
#include <stdint.h>

void pmm_new_init(uintptr_t start_addr,uint32_t length_bytes) {

    uintptr_t start = ROUND_TO_PAGES(start_addr);
    uint32_t mem_length_bytes = length_bytes & ~PAGE_SIZE;

    for (void * addr = start; addr < start+mem_length_bytes;addr+=PAGE_SIZE) {
        addr = (struct frame*) addr;
        struct frame* frame = addr;
        if(addr< (start+mem_length_bytes)) {
            frame->next = addr+PAGE_SIZE;
        }
        if(addr == start) {
            first_free_addr = addr;
        }
    }


};
