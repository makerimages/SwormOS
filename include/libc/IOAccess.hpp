#ifndef IOACCESS_H
#define IOACCESS_H
	
#include <stddef.h>
#include <stdint.h>

	extern inline void outb(uint16_t port, uint8_t val) {
		asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	}

	extern inline uint8_t inb(uint16_t port) {
    	uint8_t ret;
    	asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
   	 	
    	return ret;
	}

	extern inline void outw(uint16_t port, uint16_t val) {
		asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
	}


#endif