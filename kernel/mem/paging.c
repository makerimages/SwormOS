#include <paging.h>
#include <textmode.h>

extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
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

    kputcolor(green,black);
    kputs("OK\n");
    kputcolor(lightGrey,black);
}
