#define FRAME_BUFFER 0x000B8000
#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

#include "io.h"

/** fb_write_cell:
        *  Writes a character with the given foreground and background to position i
        *  in the framebuffer.
        *
        *  @param i  The location in the framebuffer
        *  @param c  The character
        *  @param fg The foreground color
        *  @param bg The background color
        */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
        char *fb = (char *) FRAME_BUFFER;

        fb[i] = c;
        fb[i+1] = (((fg & 0x0f) << 4) | (bg & 0x0f));
}

/* fb_clear:
        *  Clears the frame buffer (make blank screen)
        */
void fb_clear()
{
        const int size = 80 * 25;
        char *fb = (char *) FRAME_BUFFER;

        for (int i = 0; i < size; i++)
                fb[i] = 0x00;
}

/* fb_write:
        *  Writes a character buffer to the frame buffer
        *
        *  @param *buf The pointer to the buffer that is going to be written
        *  @param len  The length of the buffer
        */
void fb_write(char *buf, unsigned int len)
{
        for (unsigned int i = 0; i < len; i++)
                fb_write_cell((i*2), buf[i], FB_DARK_GREY, FB_GREEN);

        fb_move_cursor(len);
}

/* serial_write:
        *  Writes a character buffer to the a serial port
        *
        *  @param *buf The pointer to the buffer that is going to be written
        *  @param len  The length of the buffer
        */
void serial_write(char *buf, unsigned int len)
{
        for (unsigned int i = 0; i < len; i++)
                serial_write_char(SERIAL_COM1_BASE, buf[i]);
}

/* kernel main routine */
void kmain()
{
        fb_clear();
        char *greet = "Hello World!!!";
        fb_write(greet, 14);
        serial_write(greet, 14);
}


























