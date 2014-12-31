#include <include/Terminal.h>
#include <include/system.h>

Terminal::Terminal() {
}

void Terminal::Init() {
	setCursorPos(0,0);
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	fill();
	outw(0x3D4,0x200A);

}

uint8_t Terminal::make_color(enum vga_color fg, enum vga_color bg) {
		return fg | bg << 4;
}

uint16_t Terminal::make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t Terminal::strlen(const char* str) {
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

void Terminal::setColor(uint8_t color) {
		terminal_color = color;
}


void Terminal::terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * terminal_width + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void Terminal::terminal_putchar(char c) {
	if(c == '\n') {
		terminal_column = 0;
		terminal_row = terminal_row+1;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if ( ++terminal_column == terminal_width )
		{
			terminal_column = 0;
			if ( ++terminal_row == terminal_height )
			{
				terminal_row = 0;
			}
		}
	}
}

void Terminal::print(const char* data) {
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ ) {
		terminal_putchar(data[i]);
	}
}

void Terminal::setCursorPos(size_t col, size_t row) {
	terminal_column = col;
	terminal_row = row;
}
void Terminal::fill() {
	for ( size_t y = 0; y < terminal_height; y++ )
	{
		for ( size_t x = 0; x < terminal_width; x++ )
		{
			const size_t index = y * terminal_width + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}