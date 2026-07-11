#ifndef INCLUDE_MEM_INIT_H
#define INCLUDE_MEM_INIT_H

#include <stdint.h>

/** protected_mode_gdt:
        *  Sets up the GDT for protected mode segmentation
        */
void protected_mode_gdt(void);

#endif /* INCLUDE_MEM_INIT_H */
