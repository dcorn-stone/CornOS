#ifndef INCLUDE_INTERRUPTS_H
#define INCLUDE_INTERRUPTS_H

#include <stdint.h>

/* create_idt:
        *  creates a interrupt descriptor table and loads it
        */
void create_idt();

void pic_init();

#endif /* INCLUDE_INTERRUPTS_H */
