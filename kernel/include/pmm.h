#ifndef PMM_NEW_H_
#define PMM_NEW_H_

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define ROUND_TO_PAGES(Size) (((uintptr_t)(Size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

    struct frame {
        struct frame* next;
    };


    struct frame* first_free_addr;

    void pmm_init(uintptr_t start_addr,uint32_t length_bytes);
    uintptr_t alloc_phys();
    void kfree(void* address);
    void pmm_map();
#endif
