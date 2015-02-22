#ifndef IOAccess_H
#define IOAccess_H
	#include <stddef.h>
	#include <stdint.h>

	extern inline void outb(uint16_t port, uint8_t val) {
		asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	}

	extern inline unsigned char inb(unsigned short int port) {
    	unsigned char v;
    	asm volatile ( "inb %w1, %0" : "=a"(v) : "dN"(port) );
   	 	
    	return v;

	}

	extern inline void outw(uint16_t port, uint16_t val) {
		asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
	}

#endif