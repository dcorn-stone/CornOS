#include "io.h"

/* The I/O ports */

    /* All the I/O ports are calculated relative to the data port. This is because
     * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
     * order, but they start at different values.
     */


/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
        * Tells the serial port to expect first the highest 8 bits on the data port,
        * then the lowest 8 bits will follow
        */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** io_wait:
        *  Wait for a very short and imprecise amount of time
        */
void io_wait()
{
        outb(0x80, 0);
}

/** fb_move_cursor:
        *  Moves the cursor of the framebuffer to the given position
        *
        *  @param pos The new position of the cursor
        */
void fb_move_cursor(uint16_t pos)
{
        outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
        outb(FB_DATA_PORT, ((pos >> 8) & 0x00ff));
        outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
        outb(FB_DATA_PORT, (pos & 0x00ff));
}

/** serial_configure_baud_rate:
        *  Sets the speed of the data being sent. The default speed of a serial
        *  port is 115200 bits/s. The argument is a divisor of that number, hence
        *  the resulting speed becomes (115200 / divisor) bits/s.
        *
        *  @param com      The COM port to configure
        *  @param divisor  The divisor
        */
void serial_configure_baud_rate(uint16_t com, uint16_t divisor)
{
        outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
        outb(SERIAL_DATA_PORT(com), ((divisor >> 8) & 0x00ff));
        outb(SERIAL_DATA_PORT(com), (divisor & 0x00ff));
}

/** serial_configure_line:
        *  Configures the line of the given serial port. The port is set to have a
        *  data length of 8 bits, no parity bits, one stop bit and break control
        *  disabled.
        *
        *  @param com  The serial port to configure
        */
void serial_configure_line(uint16_t com)
{
        /*
         * d 	Enables (d = 1) or disables (d = 0) DLAB
         * b 	If break control is enabled (b = 1) or disabled (b = 0)
         * prty The number of parity bits to use
         * s 	The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
         * dl 	Describes the length of the data
         */

        /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
         * Content: | d | b | prty  | s | dl  |
         * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
         */
        outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_buffer:
        *  Configures the FIFO buffer. The configuration is such that
        *  - Enables FIFO
        *  - Clear both receiver and transmission FIFO queues
        *  - Use 14 bytes as size of queue
        *
        *  @param com  The serial port to configure
        */
void serial_configure_buffer(uint16_t com)
{
        /*
         *  Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
         *  Content: | lvl | bs | r | dma | clt | clr | e |
         *  Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7
         */
        outb(SERIAL_FIFO_COMMAND_PORT(com), 0xc7);
}

/** serial_configure_modem:
        *  Configures the serial modem.
        *
        *  @param com  The serial port to configure
        */
void serial_configure_modem(uint16_t com)
{
        /*  Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
         *  Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
         *  Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   | = 0x03
         */
        outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
        *  Checks whether the transmit FIFO queue is empty or not for the given COM
        *  port.
        *
        *  @param  com The COM port
        *  @return 0 if the transmit FIFO queue is not empty
        *          1 if the transmit FIFO queue is empty
        */
int serial_is_transmit_fifo_empty(uint32_t com)
{
        /* 0x20 = 0010 0000 */
        /* 5th bit of the line status byte indicates if the fifo is empty */
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** serial_write_byte:
        *  Writes a byte to a COM serial port
        *
        *  @param com The COM port
        *  @param c   The character to write to the serial port
        */
void serial_write_char(uint16_t com, char c)
{
        while(!serial_is_transmit_fifo_empty(com)); // Doing nothing until the fifo is empty

        outb(SERIAL_DATA_PORT(com), c);
}




