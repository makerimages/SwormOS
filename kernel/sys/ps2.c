#include <ps2.h>
#include <textmode.h>

void ps2_init() {
    struct FADT* fadt = (struct FADT*) find_table("FADT",get_rsdt());
    if(fadt->BootArchitectureFlags & (1 << 1)) {
        kputs("There is a PS/2 controller");
    }

}
