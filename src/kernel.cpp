#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif

#include "drivers/include/terminal.h"
#include "drivers/include/system.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */

Terminal terminal;

extern void fatalErr(const char* type) {
	Terminal termi;
	termi.Init();
	termi.setColor(termi.make_color(termi.COLOR_DARK_GREY, termi.COLOR_LIGHT_GREY));
	termi.fill();
	termi.setCursorPos((80/2)-termi.strlen("OS Zin has crashed to unrecoverable grounds")/2,(24/2)-2);
	termi.print("OS Zin has crashed to unrecoverable grounds");
	termi.setCursorPos((80/2)-termi.strlen(type)/2-3,(24/2)-1);
	termi.setColor(termi.make_color(termi.COLOR_RED, termi.COLOR_LIGHT_GREY));
	termi.print("Error: ");
	termi.print(type);
	termi.setCursorPos((80/2)-termi.strlen("Please reboot and fix the above error.")/2,23);
	termi.print("Please reboot and fix the above error.");

}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
	terminal.Init();
	terminal.print("OS Zin v 0.0.1.\n");
	terminal.print("Copyright (c) Makerimages 2014.\n");
}