#include <ps2.h>
#include <acpi.h>
#include <textmode.h>

void ps2_init() {
    struct FADT* fadt = (struct FADT*) find_table("FACP");
    if(fadt->h.Revision > 0) {
        if(fadt->BootArchitectureFlags & (1 << 1)) {
           kputs("There is a PS/2 controller");
        } else {
           kputcolor(red,black);
           kputs("Unable to find a PS/2 controller. Possible failure of features.\n");
           kputcolor(lightGrey,black);
       }
   } else {
       kputcolor(brown,black);
       kputs("ACPI revision not capable enough to determine 8042. Assuming one exists...\n");
       kputcolor(lightGrey,black);

   }



}
