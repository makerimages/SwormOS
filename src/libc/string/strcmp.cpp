#include "libc/String.hpp"

size_t strcmp(const char* str1, const char* str2) {
	size_t res = 0;
	while(!(res = *str1 - *str2) && *str2) {
		++str1, ++str2;
	}
	return res;
}