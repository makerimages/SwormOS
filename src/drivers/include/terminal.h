#ifndef TERMINAL_H
#define TERMINAL_H

#include "system.h"

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
		uint8_t make_color(enum vga_color fg, enum vga_color bg);
		void setColor(uint8_t color);
		void print(const char* data);
		void print(bool data);

		void setCursorPos(size_t col, size_t row);
		void fill();
		void fatalError(const char* type);
	protected: 
		size_t terminal_row;
		size_t terminal_column;
		uint8_t terminal_color;
		uint16_t* terminal_buffer;
		static const size_t terminal_width = 80;
		static const size_t terminal_height = 25;
		
		uint16_t make_vgaentry(char c, uint8_t color);
		void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
		void terminal_putchar(char c);
};

#endif
