#ifndef STRING_H
#define STRING_H
	#include <stddef.h>

	#include <stdint.h>

	extern "C" {
		int memcmp(const void*, const void*, size_t);
		void* memcpy(void* __restrict, const void* __restrict, size_t);
		void* memmove(void*, const void*, size_t);
		void* memset(void*, int, size_t);
		size_t strlen(const char*);
		char * itoa( int value, char * str, int base );
		char * uitoa(unsigned int value, char * str, int base );

	}
#endif