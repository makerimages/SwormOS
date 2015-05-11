#ifndef PMM_H
#define PMM_H
//! 8 blocks per byte
#define BPB 8

//! block size (4k)
#define BSZ	4096

//! block alignment
#define BALIGN	BSZ

#define physical_addr uintptr_t

#include <stdint.h>
#include <stddef.h>

class PMM {
  public:
    PMM();
    void init(size_t memSize, uint32_t bitmap);
    void* allocate();
    void deallocate(void* p);
    bool test (int bit);
    uint32_t memorySize;
    uint32_t used;
    uint32_t max;
    uint32_t* managerMap;
    void setBit (int bit);
    void unsetBit(int bit);
    int firstFree ();
    uint32_t blocks();
    void	initRegion (uint32_t base, size_t size);
    void	deinitRegion (uint32_t base, size_t size);
    void map();

};

#endif
