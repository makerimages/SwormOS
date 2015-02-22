#include "OSZin/modules/TextMode.hpp"

TextMode::TextMode() {} //Constructor is needed but cant' do anything :/

void TextMode::init() { //Initialize our Text Mode
	this->setCursorPos(0,0);
	this->setColor(lightGrey,black);
	buffer = reinterpret_cast<uint16_t*>( 0xB8000);
	this->fill();
	this->resetColor();

	//Hide cursor
	outw(0x3D4,0x200A);
	outw(0x3D4,0xB);
}

void TextMode::setColor(enum colors fg, enum colors bg) {
	color = fg | bg << 4;
}

uint16_t TextMode::vgaEntry(char c, uint8_t color) {
	uint16_t c16 = c; //Char to 16 bits
	uint16_t color16 = color; //Color to 16 bits
	return c16 | color16 << 8;
}

void TextMode::putEntryAt(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = (y*width)+x;
	buffer[index] = vgaEntry(c,color);
}

void TextMode::putChar(char c) {
	switch(c) {  //Handle special cases.
		case '\n':
			setCursorPos(0,row+1);
			break;
		case '\r':
			setCursorPos(0,row);
			break;
		case '\b':
			setCursorPos(column-1,row);
			break;
		case '\t':
			setCursorPos(column+tabSize,row);
			break;
		default:
			putEntryAt(c,color,column,row);
			setCursorPos(column+1,row);
			break;
	}
	if(column > width - 1) {
		setCursorPos(0,row+1);
	}
	if(row > height -1) {
		memmove(buffer,buffer+width,(height-1)*width*2);
		for(size_t i = 0; i < width; i++) {
			buffer[(row-1)*width+i] = vgaEntry(' ', color);
		}
		setCursorPos(0,row-1);
	}
}

void TextMode::setCursorPos(size_t c, size_t r) {
	column = c;
	row = r;
}

void TextMode::fill() {
	for ( size_t y = 0; y < height; y++ )
	{
		for ( size_t x = 0; x < width; x++ )
		{
			const size_t index = y * width + x;
			buffer[index] = vgaEntry(' ', color);
		}
	}
}

void TextMode::kputsf(const char* str, ...) {
	va_list va;
	va_start(va,str);
	kputsfImpl(str,va);
	va_end(va);
}

void TextMode::kputsfImpl(const char* str, va_list va) {
	char buf[33];
	while(*str)  {
		if(*str == '%') {
			str++;
			if(!str) {
				break;
			}
			switch(*str) {
				case 'c':
					putChar(va_arg(va,int));
					break;
				case 's' :
					kputs(va_arg(va,const char*));
					break;
				case 'b':
					kputs(itoa(va_arg(va,int),buf,2));
					break;
				case 'd':
					kputs(uitoa(va_arg(va,uint32_t),buf,10));
					break;
				case 'u':
					kputs(uitoa(va_arg(va,uint32_t),buf,10));
					break;
				case 'x':
					kputs(uitoa(va_arg(va,uint32_t),buf,16));
					break;
				default:
					break;
			} 
		} else {
			putChar(*str);
		}
		str++;
	}
}

void TextMode::kputs(const char* str) {
	kputsImpl(str);
}

void TextMode::kputsImpl(const char* str) {
	while(*str) {
		putChar(*(str++));
	}
}

void TextMode::panic(const char* msg) {
	this->setColor(this->darkGrey,this->blue);
	this->fill();
	this -> setCursorPos((width/2)-strlen("OS Zin has crashed to unrecoverable grounds")/2,(height/2)-2);
	this -> kputs("OS Zin has crashed to unrecoverable grounds");
	this -> setColor(this->red, this->blue);
	this -> setCursorPos((width/2)-strlen(msg)/2-3,(height/2)-1);
	this -> kputs("Error: ");
	this -> setCursorPos((width/2)-strlen(msg)/2+4,(height/2)-1);
	this -> kputs(msg);
	this -> setCursorPos(width/2-strlen("System halted to protect your PC")/2,height-3);
	this -> kputs("System halted to protect your PC");
	this -> setCursorPos((width/2)-strlen("Please reboot and fix the above error")/2,height-2);
	this -> kputs("Please reboot and fix the above error");
	while(true) {
		__asm__ volatile ("hlt");
	}	

}

void TextMode::resetColor() {
	setColor(lightGrey,black);
}