#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* str);
size_t strcmp(const char* str1, const char* str2);
char * uitoa(unsigned int value, char * str, int base );
char* itoa(int value, char* str, int base);
void * memmove(void * dest, const void * from, size_t size);
#endif