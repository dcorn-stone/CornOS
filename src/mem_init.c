#include "mem_init.h"


/** gdt_entry
        * Sturcture of every entry (segment descriptor) in the GDT */
struct gdt_entry {
        unsigned short limit_low; // byte 0 - 1
        unsigned short base_low; // byte 2 - 3

        unsigned char base_middle; // byte 4
        unsigned char access_byte; // byte 5

        unsigned char flags; // byte 6
                                /* Bits:    | 7    4 | 3    0     | */
                                /* Content: | Flags  | Limit high | */
        unsigned char base_high; // byte 7

} __attribute__((packed));

void protected_mode_gdt()
{
        static struct gdt_entry gdt[5];
        /* The first entry in the GDT is the Null descriptor,
                therefore everything is set to 0 */
        gdt[0].limit_low = 0x00000000;
        gdt[0].base_low = 0;
        gdt[0].base_middle = 0;
        gdt[0].access_byte = 0x00;
        gdt[0].flags = 0x00 | 0x00; // flags byte is 4 bits flags + 4 bits limit high, not 8 bits flags
        gdt[0].base_high = 0;

        /* The second entry is the kernel code segment */
        gdt[1].limit_low = 0xffff;
        gdt[1].base_low = 0;
        gdt[1].base_middle = 0;
        gdt[1].access_byte = 0x9a;
        gdt[1].flags = 0xc0 | 0x0f;
        gdt[1].base_high = 0;
        
        /* The third entry is the kernel data segment */
        gdt[2].limit_low = 0xffff;
        gdt[2].base_low = 0;
        gdt[2].base_middle = 0;
        gdt[2].access_byte = 0x92;
        gdt[2].flags = 0xc0 | 0x0f;
        gdt[2].base_high = 0;

        /* The fourth entry is the user code segment */
        gdt[3].limit_low = 0xffff;
        gdt[3].base_low = 0;
        gdt[3].base_middle = 0;
        gdt[3].access_byte = 0xfa;
        gdt[3].flags = 0xc0 | 0x0f;
        gdt[3].base_high = 0;

        /* The fifth entry is the user data segment */
        gdt[4].limit_low = 0xffff;
        gdt[4].base_low = 0;
        gdt[4].base_middle = 0;
        gdt[4].access_byte = 0xf2;
        gdt[4].flags = 0xc0 | 0x0f;
        gdt[4].base_high = 0;

        struct gdt_ptr gdt_address;
        gdt_address.address = (unsigned int) gdt;
        gdt_address.size = sizeof(gdt) - 1;

        load_gdt(&gdt_address);

        reload_segments();
}
