#include <stdint.h>

// TODO: Magic values ohoy! Fix that.

#define GRAN_64_BIT_MODE (1 << 5)
#define GRAN_32_BIT_MODE (1 << 6)
#define GRAN_4KIB_BLOCKS (1 << 7)

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
}  __attribute__((packed));

#define GDT_ENTRY(base, limit, access, granularity) \
	{ (limit) & 0xFFFF,                                /* limit_low */ \
	  (base) >> 0 & 0xFFFF,                            /* base_low */ \
	  (base) >> 16 & 0xFF,                             /* base_middle */ \
	  (access) & 0xFF,                                 /* access */ \
	  ((limit) >> 16 & 0x0F) | ((granularity) & 0xF0), /* granularity */ \
	  (base) >> 24 & 0xFF,                             /* base_high */ }

struct gdt_entry gdt[] =
{
	/* 0x00: Null segment */
	GDT_ENTRY(0, 0, 0, 0),

	/* 0x08: Kernel Code Segment. */
	GDT_ENTRY(0, 0xFFFFFFFF, 0x9A, GRAN_32_BIT_MODE | GRAN_4KIB_BLOCKS),

	/* 0x10: Kernel Data Segment. */
	GDT_ENTRY(0, 0xFFFFFFFF, 0x92, GRAN_32_BIT_MODE | GRAN_4KIB_BLOCKS),

	/* 0x18: User Code Segment. */
	GDT_ENTRY(0, 0xFFFFFFFF, 0xFA, GRAN_32_BIT_MODE | GRAN_4KIB_BLOCKS),

	/* 0x20: User Data Segment. */
	GDT_ENTRY(0, 0xFFFFFFFF, 0xF2, GRAN_32_BIT_MODE | GRAN_4KIB_BLOCKS),
};
uint16_t gdt_size_minus_one = sizeof(gdt) - 1;