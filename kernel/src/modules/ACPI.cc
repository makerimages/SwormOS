#include <modules/ACPI.hpp>
#include <KernelGlobals.hpp>

uint8_t DoChecksum(struct RSDPDescriptor*  t)
{
   uint8_t count = 0;
   const unsigned char * p = reinterpret_cast<const unsigned char *>(t);
   for (int i=0; i<sizeof(*t); i++) count += p[i];
   return count;
}

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
            tm.kputsf("ACPI signature: %sOEMID: %s\n",temp->Signature, temp->OEMID);
            tm.kputs("Verifying checksum: ");
            if(DoChecksum(temp) == 0) {
                tm.setColor(tm.green,tm.black);
                tm.kputs("OK\n");
                tm.resetColor();
                RSDPtable = temp;
                tm.kputsf("RSDT table location: 0x%x",RSDPtable->RsdtAddress);
            } else {
                tm.setColor(tm.red,tm.black);
                tm.kputs("NOT VERIFIED\n");
                tm.resetColor();
            }

        }
    }
}

void ACPI::loadRSDT() {
    
}
