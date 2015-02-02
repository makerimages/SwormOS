#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/String.h"
#include "libc/IOAccess.h"

#include <stddef.h>
#include <stdint.h>

class Terminal {
	public:
		enum vga_color
		{
			COLOR_BLACK = 0,
			COLOR_BLUE = 1,
			COLOR_GREEN = 2,
			COLOR_CYAN = 3,
			COLOR_RED = 4,
			COLOR_MAGENTA = 5,
			COLOR_BROWN = 6,
			COLOR_LIGHT_GREY = 7,
			COLOR_DARK_GREY = 8,
			COLOR_LIGHT_BLUE = 9,
			COLOR_LIGHT_GREEN = 10,
			COLOR_LIGHT_CYAN = 11,
			COLOR_LIGHT_RED = 12,
			COLOR_LIGHT_MAGENTA = 13,
			COLOR_LIGHT_BROWN = 14,
			COLOR_WHITE = 15,
		};

		Terminal(); 
		void Init();
		uint8_t makeColor(enum vga_color fg, enum vga_color bg);
		void setColor(uint8_t colors);
		void resetColor();
		void print(const char* data);
		void print(bool data);

		void setCursorPos(size_t col, size_t rw);
		void fill();
		void fatalError(const char* type);
	protected: 
		size_t row;
		size_t column;
		uint8_t color;
		uint16_t* buffer;
		static const size_t width = 80;
		static const size_t height = 25;
		
		uint16_t vgaentry(char c, uint8_t color);
		void putentryat(char c, uint8_t color, size_t x, size_t y);
		void putchar(char c);
};

#endif
