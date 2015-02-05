#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/IOAccess.hpp"
#include "kernel/KernelGlobals.hpp"

#include <stddef.h>
#include <stdint.h>

class Keyboard {
	public:
		Keyboard();
		void init();
		void writeControl(uint8_t bit);
		uint8_t readControl();
		void writeData(uint8_t bit);
		uint8_t readData();
	private:
		uint8_t config, data;
};
#endif