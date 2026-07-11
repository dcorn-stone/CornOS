#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <stdint.h>

/** fb_move_cursor:
        *  Moves the cursor of the framebuffer to the given position
        *
        *  @param pos The new position of the cursor
        */
void fb_move_cursor(uint16_t pos);

/** fb_move_cursor:
        *  Moves the cursor of the framebuffer to the given position
        *
        *  @param pos The new position of the cursor
        */
void serial_write_char(uint16_t com, char c);

/** outb:
        *  Sends the given data to the given I/O port. Defined in io.s
        *
        *  @param port The I/O port to send the data to
        *  @param data The data to send to the I/O port
        */
void outb(uint16_t port, uint8_t data);

/** inb:
 *  Read a byte from an I/O port.
 *
 *  @param  port The address of the I/O port
 *  @return      The read byte
 */
uint8_t inb(uint16_t port);

/** io_wait:
 *  Wait for a very short and imprecise amount of time
 */
void io_wait(void);

#endif /* INCLUDE_IO_H */
