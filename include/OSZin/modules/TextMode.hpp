#ifndef TextMode_H
#define TextMode_H 
#include "libc/String.hpp"
#include "libc/IOAccess.hpp"

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

class TextMode {
	public:
		enum colors { //Define all colors usable in textmode 
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

		TextMode(); //Constructor
		void init(); //Initialize the text mode
		void setColor(enum colors fg, enum colors bg);
		void resetColor();
		void kputsf(const char* str, ...);
		void kputs(const char* str);
		void kputsBool(bool val);
		void setCursorPos(size_t c, size_t r);
		void fill();
		void panic(const char* msg);
		size_t column, row;
		static const size_t width = 80;
	private:
		uint16_t vgaEntry(char c, uint8_t color);
		void putEntryAt(char c, uint8_t color, size_t x, size_t y);
		void putChar(char c);
		void kputsfImpl(const char* str, va_list va); //Named Impl because this is where the actal fnctionality is implemented in
		void kputsImpl(const char* str);
		uint8_t color;
		uint16_t* buffer;
		
		static const size_t height = 25;
		static const int tabSize = 2;

};	
#endif