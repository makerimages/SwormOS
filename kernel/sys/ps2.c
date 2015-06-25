#include <ps2.h>
#include <acpi.h>
#include <textmode.h>

void ps2_init() {
    struct FADT* fadt = (struct FADT*) find_table("FACP");
    if(fadt == NULL) {
        kputs(":(");
    }
    if(fadt->BootArchitectureFlags & (1 << 1)) {
       kputs("There is a PS/2 controller");
    }


}
