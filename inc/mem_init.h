#ifndef INCLUDE_MEM_INIT_H
#define INCLUDE_MEM_INIT_H

/* gdt: Struct containing the base address and the limit of a Global Descriptor Table */
struct gdt_ptr {
        unsigned int address; // The base address of the gdt
        unsigned short size; // The limit/size of the gdt
} __attribute__((packed));

/** load_gdt:
        *  Loads the Global Descriptor Table
        *
        *  @param *gdt The struct pointer to the GDT with its base address and limit
        */
void load_gdt(struct gdt_ptr *gdt);

/** reload_segments:
        *  Reloads the segment registers
        */
void reload_segments();

#endif /* INCLUDE_MEM_INIT_H */
