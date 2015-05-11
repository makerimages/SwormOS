#include <arch/ia32/modules/PMM.hpp>
#include <KernelGlobals.hpp>
#include <stdint.h>
#include <string.h>

PMM::PMM () {} //Constructor is needed but cant' do anything :/

void PMM::init(size_t memSize, uint32_t bitmap) {

  memorySize	=	memSize;

	max	=	(memSize*1024) / BSZ;
	used =	this->max;

	//! By default, all of memory is in use
	memset (managerMap, 0xf, max / BPB );
  managerMap	=	(uint32_t*) bitmap;
  initRegion (0x100000, memSize*512);
  tm.setColor (tm.green, tm.black);
  tm.kputsf("\tPMM initialized with %d blocks of memory. %d currently in use\n",max,used);
  tm.resetColor();
}

void PMM::setBit(int bit) {
  managerMap[bit / 32] |= (1 << (bit % 32));
}

void PMM::unsetBit(int bit) {
  managerMap[bit / 32] &= ~ (1 << (bit % 32));
}


bool PMM::test(int bit) {
  return managerMap[bit / 32] &  (1 << (bit % 32));
}

int PMM::firstFree() {
  //! find the first free bit
	for (uint32_t i=0; i< blocks() / 32; i++) {
		if (managerMap[i] != 0xffffffff) {
			for (int j=0; j<32; j++) {		//! test each bit in the dword

				int bit = 1 << j;
				if (! (managerMap[i] & bit) ) {
					return i*4*8+j;
        }
      }
		}
  }

	return -1;
}

uint32_t PMM::blocks() {
  return max;
}


void PMM::initRegion (uint32_t base, size_t size) {
  int align = base / BSZ;
	int blocks = size / BSZ;

	for (; blocks>0; blocks--) {
		unsetBit(align++);
		used--;
	}

	setBit(0);	//first block is always set. This insures allocs cant be 0
}

void PMM::deinitRegion(uint32_t base, size_t size) {
  int align = base / BSZ;
	int blocks = size / BSZ;

	for (; blocks>0; blocks--) {
		setBit(align++);
		used++;
	}
}

void* PMM::allocate() {

	if (max-used <= 0)
		return 0;	//out of memory

	int frame = firstFree();

	if (frame == -1)
		return 0;	//out of memory

  setBit(frame);

  physical_addr addr = frame * BSZ;
	used++;

	return (void*)addr;
}

void PMM::deallocate(void* p) {

  uint32_t addr = reinterpret_cast<uint32_t> (p);
	int frame = addr / BSZ;

	unsetBit(frame);

	used--;

}

void PMM::map() {
  tm.setColor (tm.lightBlue, tm.black);

  tm.kputsf("\tPMM status: First free: %d, used: %d, max: %d.\n",firstFree(),used,max);
  tm.resetColor();
}
