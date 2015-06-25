#ifndef IO_H_
#define IO_H_
#include <stdint.h>

    static inline void outb(uint16_t port, uint8_t val)
    {
        __asm__ __volatile__ ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    };

    static inline void outw(uint16_t port, uint16_t val)
    {
        __asm__ __volatile__ ( "outw %0, %1" : : "a"(val), "Nd"(port) );
    };

    static inline void outl(uint16_t port, uint32_t val)
    {
        __asm__ __volatile__ ( "outl %0, %1" : : "a"(val), "Nd"(port) );
    };

#endif
