#include "drivers/Terminal.hpp"

Terminal::Terminal() {
}

void Terminal::Init() {
	setCursorPos(0,0);
	color = makeColor(COLOR_LIGHT_GREY, COLOR_BLACK);
	buffer = (uint16_t*) 0xB8000;
	fill();
	outw(0x3D4,0x200A);

}

uint8_t Terminal::makeColor(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

uint16_t Terminal::vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void Terminal::setColor(uint8_t colors) {
	color = colors;
}


void Terminal::putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = (y * width) + x;
	buffer[index] = vgaentry(c, color);
}

void Terminal::putchar(char c) {
	if (c == '\n') {
		column = 0;
		row++;
	} else if (c == '\r')
	column = 0;
	else if (c == '\b') {
		column--;
		if (column < 0)
			column = 0;
	} else if (c == '\t')
			column = (column+tabSize) & ~tabSize;
	else {
		putentryat(c,color,column,row);
		column++;
	}
	if (column > width - 1) {
		row++;
		column -= width;
	}
	if (row > height - 1) {
		memmove(buffer, buffer+width, height-1*width*2);
		for (int i = 0; i < width; i++) {
			buffer[(height-1)*width+i] = vgaentry(' ',color);

		}
		row--;
	}
}

void Terminal::print(const char* data) {
		size_t datalen = strlen(data);
		for ( size_t i = 0; i < datalen; i++ ) {
			putchar(data[i]);
		}
	

}

void Terminal::print(bool data) {
	const char* str;
	if(data) {
		str = "True";
		setColor(makeColor(COLOR_GREEN,COLOR_BLACK));
	}
	else {
		str = "False";
		setColor(makeColor(COLOR_RED,COLOR_BLACK));
	}
	if (row+1 >= height )
	{
		row = 0;
		column = 0;
		size_t datalen = strlen(str);
		for ( size_t i = 0; i < datalen; i++ ) {
			putchar(str[i]);
		}

	} else {
		size_t datalen = strlen(str);
		for ( size_t i = 0; i < datalen; i++ ) {
			putchar(str[i]);
		}
	}
	color = makeColor(COLOR_LIGHT_GREY, COLOR_BLACK);

}


void Terminal::setCursorPos(size_t col, size_t rw) {
	column = col;
	row = rw;
}

void Terminal::fill() {
	for ( size_t y = 0; y < height; y++ )
	{
		for ( size_t x = 0; x < width; x++ )
		{
			const size_t index = y * width + x;
			buffer[index] = vgaentry(' ', color);
		}
	}
}

void Terminal::fatalError(const char* type) {

	this -> setColor(makeColor(this -> COLOR_DARK_GREY, this -> COLOR_LIGHT_GREY));
	this -> fill();
	this -> setCursorPos((width/2)-strlen("OS Zin has crashed to unrecoverable grounds")/2,(height/2)-2);
	this -> print("OS Zin has crashed to unrecoverable grounds");
	this -> setColor(this -> makeColor(this -> COLOR_RED, this -> COLOR_LIGHT_GREY));
	this -> setCursorPos((width/2)-strlen(type)/2-3,(height/2)-1);
	this -> print("Error: ");
	this -> setCursorPos((width/2)-strlen(type)/2+4,(height/2)-1);
	this -> print(type);
	this -> setCursorPos(width/2-strlen("System halted to protect your PC")/2,height-3);
	this -> print("System halted to protect your PC");
	this -> setCursorPos((width/2)-strlen("Please reboot and fix the above error")/2,height-2);
	this -> print("Please reboot and fix the above error");
	__asm__("hlt");

}

void Terminal::resetColor() {
	setColor(makeColor(COLOR_LIGHT_GREY, COLOR_BLACK));
}