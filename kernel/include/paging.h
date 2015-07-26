#ifndef PAGING_H_
#define PAGING_H_

#include <pmm.h>

    typedef struct page {
        uint32_t present  :1; // Present in memory
        uint32_t rw       :1; // Read-only if clear
        uint32_t user     :1; // Supervisor level if clear
        uint32_t accessed :1; // Accessed since last refresh?
        uint32_t dirty    :1; // Been written since last refresh?
        uint32_t unused   :7; // Unused and reserved bits
        uint32_t frame    :20; // Phys Frame address shifted right 12 bits ?

    };

    

    uint32_t* page_directory;
    uint32_t* first_page_table;

    void init_paging();
    uintptr_t get_physaddr(uintptr_t virtualaddr);
#endif
