#ifndef ACPIK_H
#define ACPIK_H

#include <stdint.h>

struct RSDPDescriptor {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__ ((packed));

struct RSDPDescriptor20 {
 RSDPDescriptor preDates;

 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};

struct RSDT {
  struct ACPISDTHeader h;
  uint32_t PointerToOtherSDT;
};

class ACPI {
    public:
        ACPI();
        int init();
    private:
        int use;
        RSDT *RSDtable;
        RSDPDescriptor *RSDPtable;
        RSDPDescriptor20 *RSDP20table;
        RSDT *rsdt;
        void loadRSDT();

};

#endif