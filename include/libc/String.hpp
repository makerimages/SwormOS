#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
char * uitoa(unsigned int value, char * str, int base );
void * memmove(void * dest, const void * from, size_t size);
#endif