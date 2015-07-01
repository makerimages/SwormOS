#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <ps2.h>
#include <idt.h>
#include <stddef.h>
#include <stdbool.h>

    bool done;


    unsigned char previous;
    char key_last_released;
    unsigned char  scancode;
    void init_keyboard();
#endif
