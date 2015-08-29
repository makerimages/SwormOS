#include <acpi.h>
#include <stdint.h>
#include <textmode.h>

uint8_t check_rsdp(struct RSDPDescriptor* t) {
    uint8_t count = 0;
    const unsigned char * p = (const unsigned char*) t;
    for (int i=0; i<20; i++) count += p[i];
    return count;
}

bool check_std(struct ACPISDTHeader* tableHeader) {
    unsigned char sum = 0;

  for (int i = 0; i < tableHeader->Length; i++)
  {
      sum += ((char *) tableHeader)[i];
  }

  return sum == 0;
}

void EBDA_range() {
    for(loc=0x9FC000; loc<= 0xA0000; loc+=0x10) {
    if(!memcmp(loc,"RSD PTR ", 8)) {
        kputcolor(green,black);
        kprintf("'RSD PTR ' located at: 0x%x\n",loc);
        kputcolor(lightGrey,black);
        found = 1;
        break;
    }
  }
}

void RSD_mem_range() {
    if(!found) {
    for(loc=0x000E0000; loc<= 0x0010000000; loc+=0x10) {
      if(!memcmp(loc,"RSD PTR ", 8)) {
          kputcolor(green, black);
          kprintf("'RSD PTR ' located at: 0x%x\n",loc);
          kputcolor(lightGrey,black);
          struct RSDPDescriptor *temp = (struct RSDPDescriptor *)loc;
          if(check_rsdp(temp) == 0) {
            found = 1;
            break;
          }
      }
    }
  }
}

void load_RSDT() {
    EBDA_range();
    RSD_mem_range();
}

void acpi_init() {
    found = 0;
    kputs("Initializing ACPI services...\n");
    load_RSDT();
    if(!found) {
        kputcolor(red, black);
        kputs("'RSD PTR ' not found in the memory range.\n");
        kputcolor(lightGrey,black);
    } else {
        struct RSDPDescriptor *temp = (struct RSDPDescriptor *)loc;
        if(temp -> Revision == 0) { // We can continue with the temp table :P
            kputs("Using RSDP Table pre-2.0.\n");
            use = 1;
            kprintf("ACPI signature: %sOEMID: %s\n",temp->Signature, temp->OEMID);
            kputs("Verifying checksum: ");
            if(check_rsdp(temp) == 0) {
                kputcolor(green,black);
                kputs("OK\n");
                kputcolor(lightGrey,black);
                RSDPtable = temp;
                kprintf("RSDT table location: 0x%x\n",RSDPtable->RsdtAddress);
                rsdt = (struct RSDT *) RSDPtable->RsdtAddress;
                kprintf("RSDT ACPISDTHeader Signature: %s\n",rsdt->h.Signature);
                kprintf("Verifying yet another checksum. ");
                if(check_std((struct ACPISDTHeader*) rsdt)) {
                    kputcolor(green,black);
                    kputs("OK\n");
                    kputcolor(lightGrey,black);
                    kprintf("ACPI Revision: %d.\n",rsdt->h.Revision);
                    kprintf("APIC: 0x%x\n",find_table("APIC"));
                } else {
                    kputcolor(red,black);
                    kputs("NOT VERIFIED, CAN'T CONTINUE!\n");
                    kputcolor(lightGrey,black);
                }
            } else {
                kputcolor(red, black);
                kputs("NOT VERIFIED, CAN'T CONTINUE!\n");
                kputcolor(lightGrey,black);
            }
        }
    }
    kputcolor(green,black);
    kputs("ACPI initialized.\n");
    kputcolor(lightGrey,black);
}
void* find_table(char * signature) {
  int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
  for (int i = 0; i < entries; i++)
  {

    struct ACPISDTHeader *h = (struct ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
    if (!strncmp(h->Signature, signature, 4))
          return (void *) h;
  }
  return NULL;
};

void* get_rsdt() {
    return rsdt;
};
