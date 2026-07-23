#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

#include "interrupts.h"

typedef enum{
        PANIC_DIVIDE_BY_ZERO,
        PANIC_INVALID_OPCODE,
        PANIC_DOUBLE_FAULT,
        PANIC_GENERAL_PROTECTION_FAULT,
        PANIC_PAGE_FAULT,
        PANIC_OUT_OF_MEMORY,
        PANIC_UNREACHABLE,
} panic_source_t;

/* panic:
 *      Halts the system and dumps the current states of the system
 *      depending on what type of panic it is
 *
 *      @param 
 */
void panic(panic_source_t reason, interrupt_stack_state_t *info);


#endif /* INCLUDE_ERROR_H */
