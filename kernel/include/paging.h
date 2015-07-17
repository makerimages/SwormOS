#ifndef PAGING_H_
#define PAGING_H_

#include <pmm.h>

    uint32_t* page_directory;
    uint32_t * first_page_table;

    void init_paging();

#endif
