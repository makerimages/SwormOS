#ifndef KERNEL_GLOBALS_H
#define KERNEL_GLOBALS_H
#include "drivers/Terminal.hpp"

	extern "C" uint32_t get_ebp(void);
	extern Terminal terminal;

#endif /* KERNEL_GLOBALS_H */