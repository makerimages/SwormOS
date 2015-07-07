#include <pmm_new.h>
#include <textmode.h>
#include <stdint.h>

void pmm_new_init(uintptr_t start_addr,uint32_t length_bytes) {
    kputs("\tInitializing PMM ");
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
    kputcolor(green,black);
    kputs("OK\n");
    kputcolor(lightGrey,black);
};

void * kmalloc(uint32_t size) {
    void* loc = first_free_addr;
    first_free_addr = first_free_addr->next;
    return loc;
};

void kfree(void* address) {
    struct frame* page = (struct frame*) address;
    page -> next = first_free_addr;
    first_free_addr = page;
};

void pmm_new_map() {
    kputcolor(lightBrown,black);
    kprintf("First free page is at: 0x%x. Next: 0x%x.\n",first_free_addr,first_free_addr->next);
    kputcolor(lightGrey,black);
}
