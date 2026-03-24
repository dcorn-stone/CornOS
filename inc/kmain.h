#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H


/** pause:
        *  A infinite loop to keep the kernel alive
        *  Implemented in src/asm_functions.s
        */
void pause();

/** cstrlen:
        *  Count length of a charater buffer
        *
        *  @param *str  The buffer to be counted
        *  @return  The length of the buffer passed
        */
unsigned int cstrlen(const char *str);

/** cprint:
        *  Writes a buffer to a desination depending on the selected mode
        *
        *  @param pos  Starting index in the frame buffer, not used when mode == 1
        *  @param *buf The buffer that is going to be written out
        *  @param mode The selection of output mode:
        *  0 - for writing to frame buffer
        *  1 - for writing to serial port
        *  2 - for writing to both
        *
        *  @return 0 If executed normally
        *          1 If the given mode number is not in range
        */
int cprint(unsigned int pos, const char *buf, unsigned char mode);




#endif /* INCLUDE_MAIN_H */
