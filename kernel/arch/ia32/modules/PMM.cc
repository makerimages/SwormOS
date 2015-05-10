#include <arch/ia32/modules/PMM.hpp>

#include <stdint.h>
#include <string.h>

PMM::PMM () {} //Constructor is needed but cant' do anything :/

void PMM::init(size_t memSize, uint32_t bitmap) {

  memorySize	=	memSize;
  managerMap	=	(uint32_t*) bitmap;
	max	=	(memSize*1024) / BSZ;
	used =	this->max;

	//! By default, all of memory is in use
	memset (managerMap, 0xf, max / BPB );
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
