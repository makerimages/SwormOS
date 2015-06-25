#ifndef TEXTMODE_H_
#define TEXTMODE_H_

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

    typedef enum colors { //Define all colors usable in textmode
			black = 0,
			blue = 1,
			green = 2,
			cyan = 3,
			red = 4,
			magenta = 5,
			brown = 6,
			lightGrey = 7,
			darkGrey = 8,
			lightBlue = 9,
			lightGreen = 10,
			lightCyan = 11,
			lightRed = 12,
			lightMagenta = 13,
			lightBrown = 14,
			white = 15
	};

    void textmode_init (size_t w, size_t h);
    void textmode_clear ();
    void kprintf (const char* str, ...);
    void kprintf_va (const char* str, va_list va);
    void kputc (char c);
    void kputs (const char* str);
    void kputcolor (enum colors fg, enum colors bg);
    uint16_t vga_entry (char c, uint8_t color);
    void set_pos(size_t c, size_t r);

    uint8_t color;
	uint16_t* buffer;
    size_t width;
    size_t height;
    size_t column;
    size_t row;
    int tab_size;

#endif
