#include <multiboot.h>
#include <textmode.h>

#include <stdint.h>



void kernel_main(multiboot_info_t *mbt, unsigned int magic) {
    textmode_init(80,25);
}
