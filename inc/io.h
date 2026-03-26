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

/** outb:
        *  Sends the given data to the given I/O port. Defined in io.s
        *
        *  @param port The I/O port to send the data to
        *  @param data The data to send to the I/O port
        */
void outb(unsigned short port, unsigned char data);

/** inb:
        *  Read a byte from an I/O port.
        *
        *  @param  port The address of the I/O port
        *  @return      The read byte
        */
unsigned char inb(unsigned short port);

/** io_wait:
        *  Wait for a very short and imprecise amount of time
        */
void io_wait();

#endif /* INCLUDE_IO_H */
