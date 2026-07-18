#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

#include <stdint.h>

#define TIMEOUT 20

/** pause:
        *  A infinite loop to keep the kernel alive
        *  Implemented in src/asm_functions.s
        */
void pause(void);

/** cstrlen:
        *  Count length of a charater buffer
        *
        *  @param *str  The buffer to be counted
        *  @return  The length of the buffer passed
        */
uint32_t cstrlen(const char *str);

/* kprintf:
 *      Prints to the VGA frame buffer and serial port COM1 with format
 *
 *      @param *format  A string describing the format of the target to be print
 *              ...     Additional arguments to fill in placeholders
 */
void kprintf(const char *format, ...);

#endif /* INCLUDE_MAIN_H */
