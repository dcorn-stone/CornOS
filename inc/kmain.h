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


/** fb_clear:
        *  Clears the frame buffer (make blank screen)
        */
void fb_clear(void);


#endif /* INCLUDE_MAIN_H */
