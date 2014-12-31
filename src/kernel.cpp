#include "drivers/include/terminal.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */

Terminal terminal;


#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
	terminal.Init();
	terminal.print("OS Zin v 0.0.1.\n");
	terminal.print("Copyright (c) Makerimages 2014.\n");
}