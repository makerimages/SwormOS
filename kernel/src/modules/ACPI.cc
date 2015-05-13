#include <modules/ACPI.hpp>
#include <KernelGlobals.hpp>

ACPI::ACPI() {

}

ACPI::init() {
    int found = 0;
    tm.kputs("Started sequence for initializing ACPI.\n");
    char *loc;
    for(loc=0x000E0000; loc<= 0x0010000000; loc+=0x10) {
    	if(!memcmp(loc,"RSD PTR ", 8)) {
            tm.setColor (tm.green, tm.black);
    	    tm.kputsf("'RSD PTR ' located at: 0x%x\n",loc);
            tm.resetColor();
            found = 1;
    		break;
    	}
    }
    if(!found) {
        tm.setColor (tm.red, tm.black);
        tm.kputs("'RSD PTR ' not found in the memory range.\n");
        tm.resetColor();
    } else {
        RSDPDescriptor *temp = (RSDPDescriptor *)loc;
        tm.kputsf("ACPI ver: %d\n",temp->Revision);
        if(temp -> Revision == 0) { // We can continue with the temp table :P
            tm.kputs("Using RSDP Table pre-2.0.\n");
            use = 1;
            RSDPtable = temp;
            tm.kputsf("ACPI signature: %sOEMID: %s\n",RSDPtable->Signature, RSDPtable->OEMID);
            tm.kputs("Verifiying checksum: ");
        }
    }
}
