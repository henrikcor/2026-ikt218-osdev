#include "gdt.h"



struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed)); // force exact byte layout with no padding

struct gdt_descriptor {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)); // force exact byte layout with no padding

struct gdt_entry gdt[3]; // null, kernel code, kernel data
struct gdt_descriptor gdtr;  // contains GDT size and base address, used by lgdt

extern void gdt_flush(void); // load the new GDT and reload CPU segment registers


// Fills one descriptor entry in the Global Descriptor Table.
static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran) {
    // descriptor base address
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // descriptor limit
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // upper nibble of granularity contains the flags
    gdt[num].granularity |= gran & 0xF0;

    // access byte defines the segment type and permissions
    gdt[num].access = access;
}

// Initializes the GDT with three entries: null, kernel code, and kernel data.
void gdt_install(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);             // Null descriptor
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF); // kernel code segment, base 0, 4 GB limit
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF); // Kernel data segment, base 0, 4 GB limit

    gdt_flush(); // load the new GDT and reload CPU segment registers
}

