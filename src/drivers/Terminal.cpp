#include "drivers/Terminal.hpp"
#include "modules/Elf.hpp"

Terminal::Terminal() {
}

void Terminal::Init() {
	setCursorPos(0,0);
	color = makeColor(COLOR_LIGHT_GREY, COLOR_BLACK);
	buffer = (uint16_t*) 0xB8000;
	fill();
	resetColor();

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
	switch(c) {
		case '\n':
			column = 0;
			row++;
		break;
		case '\r':
			column = 0;
		break;
		case '\b':
			column--;
			if (column < 0) {
				column = 0;
			}
		break;
		case '\t':
				column = (column+tabSize) & ~tabSize;
		break;
		default:
			putentryat(c,color,column,row);
			column++;
		break;
	}
	
		
	
	if (column > width - 1) {
		row++;
		column = 0;
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

void Terminal::printBool(bool data) {
	const char* str;
	if(data) {
		str = "True";
		setColor(makeColor(COLOR_GREEN,COLOR_BLACK));
	}
	else {
		str = "False";
		setColor(makeColor(COLOR_RED,COLOR_BLACK));
	}
	print(str);
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


void Terminal::kprintf(const char* str, ...) {
	va_list va;
	va_start(va,str);
	kprintf_(str,va);
	va_end(va);
}

void Terminal::kprintf_(const char* str, va_list va) {
	char buf[64] = "" ;
	while(*str)  {
		if(*str == '%') {
			str++;
			if(!str) {
				break;
			}
			switch(*str) {
				case 'c':
					putchar(va_arg(va,int));
					break;
				case 's' :
					kputs(va_arg(va,const char*));
					break;
				case 'b':
					kputs(itoa(va_arg(va,int),buf,2));
					break;
				case 'd':
					kputs(itoa(va_arg(va,int),buf,10));
					break;
				case 'x':
					kputs(uitoa(va_arg(va,uint32_t),buf,16));
					break;
				default:
					break;
			} 
		} else {
			putchar(*str);
		}
		str++;
	}
}

void Terminal::kputs(const char* str) {
	kputs_(str);
}

void Terminal::kputs_(const char* str) {
	while(*str) {
		putchar(*(str++));
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
	this -> setCursorPos(width/2,0);
	elf_printStackTrace();
	while(true) {
		__asm__ volatile ("hlt");

	}

}

void Terminal::resetColor() {
	setColor(makeColor(COLOR_LIGHT_GREY, COLOR_BLACK));
}