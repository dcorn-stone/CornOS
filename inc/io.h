#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** fb_move_cursor:
        *  Moves the cursor of the framebuffer to the given position
        *
        *  @param pos The new position of the cursor
        */
void fb_move_cursor(unsigned short pos);

/** fb_move_cursor:
        *  Moves the cursor of the framebuffer to the given position
        *
        *  @param pos The new position of the cursor
        */
void serial_write_char(unsigned short com, char c);


#endif /* INCLUDE_IO_H */
