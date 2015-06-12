#include <modules/ACPI.hpp>
#include <KernelGlobals.hpp>

uint8_t ACPI::RSDPChecksum(struct RSDPDescriptor*  t)
{
   uint8_t count = 0;
   const unsigned char * p = reinterpret_cast<const unsigned char *>(t);
   for (int i=0; i<20; i++) count += p[i];
   return count;
}

ACPI::ACPI() {

}

bool ACPI::STDHeaderChecksum(ACPISDTHeader *tableHeader)
{
    unsigned char sum = 0;

    for (int i = 0; i < tableHeader->Length; i++)
    {
        sum += ((char *) tableHeader)[i];
    }

    return sum == 0;
}

void* ACPI::findTable(char* signature, void* RootSDT) {
  RSDT *rsdt = (RSDT *) RootSDT;
  int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
  for (int i = 0; i < entries; i++)
  {

    ACPISDTHeader *h = (ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
    if (!strncmp(h->Signature, signature, 4))
          return (void *) h;
  }
  return NULL;
}
void ACPI::loadRSDT() {
  EBDARange();
  RSDMemRange();
}

void ACPI::EBDARange() {
  for(loc=0x9FC000; loc<= 0xA0000; loc+=0x10) {
    if(!memcmp(loc,"RSD PTR ", 8)) {
        tm.setColor (tm.green, tm.black);
        tm.kputsf("'RSD PTR ' located at: 0x%x\n",loc);
        tm.resetColor();
        found = 1;
        break;
    }
  }
}

void ACPI::RSDMemRange() {
  if(!found) {
    for(loc=0x000E0000; loc<= 0x0010000000; loc+=0x10) {
      if(!memcmp(loc,"RSD PTR ", 8)) {
          tm.setColor (tm.green, tm.black);
          tm.kputsf("'RSD PTR ' located at: 0x%x\n",loc);
          tm.resetColor();
          RSDPDescriptor *temp = (RSDPDescriptor *)loc;
          if(RSDPChecksum(temp) == 0) {
            found = 1;
            break;
          }
      }
    }
  }
}

void ACPI::init() {
    tm.kputs("Initializing ACPI services...\n");
    loadRSDT();
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
            if(RSDPChecksum(temp) == 0) {
                tm.setColor(tm.green,tm.black);
                tm.kputs("OK\n");
                tm.resetColor();
                RSDPtable = temp;
                tm.kputsf("RSDT table location: 0x%x\n",RSDPtable->RsdtAddress);
                rsdt = (RSDT *) RSDPtable->RsdtAddress;
                tm.kputsf("RSDT ACPISDTHeader Signature: %s\n",rsdt->h.Signature);
                tm.kputsf("Verifying yet another checksum. ");
                if(STDHeaderChecksum(reinterpret_cast<ACPISDTHeader*> (rsdt))) {
                  tm.setColor(tm.green,tm.black);
                  tm.kputs("OK\n");
                  tm.resetColor();
                  tm.kputsf("APIC: 0x%x\n",findTable("APIC",rsdt));
                } else {
                  tm.setColor(tm.red,tm.black);
                  tm.kputs("NOT VERIFIED, CAN'T CONTINUE!\n");
                  tm.resetColor();

                }
            } else {
                tm.setColor(tm.red,tm.black);
                tm.kputs("NOT VERIFIED, CAN'T CONTINUE!\n");
                tm.resetColor();
            }

        }
    }
}
