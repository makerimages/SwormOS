#ifndef PS2_H_
#define PS2_H_
#include <stdbool.h>

    bool isDual;
    bool failedOnce;
    bool port1OK;
    bool port2OK;
    void ps2_init();
    void ps2_setup();
    void ps2_wait();
    void ps2_wait_input();

#endif
