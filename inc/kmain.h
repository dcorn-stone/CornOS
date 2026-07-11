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
unsigned int cstrlen(const char *str);

/** kprintf:
        *  Writes a buffer to a desination depending on the selected mode
        *
        *  @param *row The pointer to the row number stored, not used when mode == 1
        *  @param *col The pointer to the column number stored, not used when mode == 1
        *  @param *buf The buffer that is going to be written out
        *  @param mode The selection of output mode:
        *  0 - for writing to frame buffer
        *  1 - for writing to serial port
        *  2 - for writing to both
        *
        *  @return 0 If finished normally
        *          1 If the given mode number is not in range
        */
int kprintf(uint32_t *row, uint32_t *col, const char *buf, uint8_t mode);

/** fb_clear:
        *  Clears the frame buffer (make blank screen)
        */
void fb_clear(void);


#endif /* INCLUDE_MAIN_H */
