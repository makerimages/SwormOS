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

    } page_t;

    typedef struct page_table {
        page_t pages[1024];
    } page_table_t;

    typedef struct page_directory {
        page_table_t *tables[1024];
        uint32_t tablesPhysical[1024];
        uint32_t physicalAddr;
    } page_directory_t;

    page_directory_t current_directory ;

    void init_paging();
    void switch_pagedir(page_directory_t* new);
    page_t * get_page(uint32_t address, int make, page_directory_t dir);
    void map_to_phys(page_t *page, int is_kernel, int is_writeable);
    void unmap_from_phys(page_t *page);
#endif
