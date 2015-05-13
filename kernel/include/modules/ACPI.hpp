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

class ACPI {
    public:
        ACPI();
        int init();
    private:
        int use;
        RSDPDescriptor *RSDPtable;
        RSDPDescriptor20 *RSDP20table;

};

#endif
