#include <pmm.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <textmode.h>

void pmm_init(size_t memSize, uint32_t bitmap) {
    memory_size = memSize;

    max	= (memSize*1024) / BSZ;
	used = max;
	memset (manager_map, 0xf, max / BPB );
    manager_map = (uint32_t*) bitmap;
    kputcolor(green, black);
    kprintf("\tPMM initialized with %d blocks of memory. %d currently in use\n",max,used);
    kputcolor(lightGrey,black);
};

void set_bit(int bit) {
  manager_map[bit / 32] |= (1 << (bit % 32));
}

void unset_bit(int bit) {
  manager_map[bit / 32] &= ~ (1 << (bit % 32));
}

bool pmm_test(int bit) {
  return manager_map[bit / 32] &  (1 << (bit % 32));
}

int first_free() {
  //! find the first free bit
  for (uint32_t i=0; i< blocks() / 32; i++) {
      if (manager_map[i] != 0xffffffff) {
          for (int j=0; j<32; j++) {		//! test each bit in the dword
              int bit = 1 << j;
              if (! (manager_map[i] & bit) ) {
                  return i*4*8+j;
              }
          }
	}
  }
  return -1;
}

uint32_t blocks() {
  return max;
}

void init_region (uint32_t base, size_t size) {
  int align = base / BSZ;
  int blocks = size / BSZ;
  for (; blocks>0; blocks--) {
      unset_bit(align++);
      used--;
  }
  set_bit(0);	//first block is always set. This insures allocs cant be 0
}

void deinit_region(uint32_t base, size_t size) {
  int align = base / BSZ;
  int blocks = size / BSZ;
  for (; blocks>0; blocks--) {
	set_bit(align++);
	used++;
  }
}

void* pmm_allocate() {
    if (max-used <= 0)
        kpanic("Out of memory.");

    int frame = first_free();
    kprintf("Frame: %d\n",frame);
    if (frame == -1)
        kpanic("Out of memory.");

    set_bit(frame);

    physical_addr addr = frame * BSZ;
	used++;

	return (void*)addr;
}

void pmm_deallocate(void* p) {
    uint32_t addr = (uint32_t) p;
	int frame = addr / BSZ;

	unset_bit(frame);

	used--;

}

void pmm_map() {
    kputcolor(lightBrown, black);
    kprintf("\tPMM status: First free: %d, used: %d, max: %d.\n",first_free(),used,max);
    kputcolor(lightGrey,black);
}
