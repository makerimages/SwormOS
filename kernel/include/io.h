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

    static inline uint8_t inb(uint16_t port)
    {
        uint8_t ret;
        __asm__ __volatile__ ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
        /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
        /* TODO: Should %1 be %w1? */
        return ret;
    }
    static inline uint16_t inw(uint16_t port)
    {
        uint16_t ret;
        __asm__ __volatile__ ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
        /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
        /* TODO: Should %1 be %w1? */
        return ret;
    }
    static inline uint32_t inl(uint16_t port)
    {
        uint32_t ret;
        __asm__ __volatile__ ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
        /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
        /* TODO: Should %1 be %w1? */
        return ret;
    }

#endif
