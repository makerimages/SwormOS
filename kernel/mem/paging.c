#include <paging.h>
#include <textmode.h>
#include <pic.h>
#include <idt.h>

static void pagefault_callback(interrupt_context * regs) {
	kpanic("Page fault!");
}

void init_paging() {
    kputs("Initializing paging... ");
    page_directory = alloc_phys();
    for(int i = 0; i < 1024; i++) {
        page_directory[i]=0x00000002;
    }
    first_page_table = alloc_phys();
    for(unsigned int j = 0; j<1024; j++) {
        first_page_table[j] = (j*0x1000) | 3;
    }
    page_directory[0] = ((unsigned int) first_page_table) | 3;
    __asm__("mov %0, %%cr3":: "r"(page_directory):"memory");

    uint32_t cr0;
    __asm__("mov %%cr0,%0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__("mov %0, %%cr0"::"r"(cr0):"memory");

    set_handler(0,14,&pagefault_callback);
    kputcolor(green,black);
    kputs("OK\n");
    kputcolor(lightGrey,black);
}
/*
uintptr_t get_physaddr(uintptr_t virtualaddr) {
    unsigned long pdindex = (unsigned long) virtualaddr >> 22;
    unsigned long ptindex = (unsigned long) virtualaddr >>  12 & 0x03FF;
    unsigned long pd = (unsigned long *) 0xFFFFF000;
};
*/
