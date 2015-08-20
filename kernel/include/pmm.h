#ifndef PMM_H_
#define PMM_H_

//! 8 blocks per byte
#define BPB 8

//! block size (4k)
#define BSZ	4096

//! block alignment
#define BALIGN	BSZ

#define physical_addr uintptr_t

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


    void pmm_init(size_t memSize, uint32_t bitmap);
    void* pmm_allocate();
    void pmm_deallocate(void* p);
    bool pmm_test (int bit);
    uint32_t memory_size;
    uint32_t used;
    uint32_t max;
    uint32_t* manager_map;
    void set_bit (int bit);
    void unset_bit(int bit);
    int first_free ();
    uint32_t blocks();
    void init_region (uint32_t base, size_t size);
    void deinit_region (uint32_t base, size_t size);
    void map();
    void deinit_bitmap();



#endif
