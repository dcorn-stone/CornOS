#define FRAME_BUFFER 0x000B8000
#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define VGA_WIDTH 80

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
#include "mem_init.h"

/** cstrlen:
        *  Count length of a charater buffer
        *
        *  @param *str  The buffer to be counted
        *  @return  The length of the buffer passed
        */
unsigned int cstrlen(const char *str)
{
        unsigned int len = 0;

        while (str[len] != '\0')
                len++;

        return len;
}

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

/** fb_clear:
        *  Clears the frame buffer (make blank screen)
        */
void fb_clear()
{
        const int size = 80 * 25;
        char *fb = (char *) FRAME_BUFFER;

        for (int i = 0; i < size; i++)
                fb[i] = 0x00;
}

/** fb_write:
        *  Writes a character buffer to the frame buffer
        *
        *  @param pos  The index +in the frame buffer that is starting to be written
        *  @param *buf The pointer to the buffer that is going to be written out
        *  @param len  The length of the buffer
        */
void fb_write(unsigned int pos, const char *buf, unsigned int len)
{
        for (unsigned int i = 0; i < len; i++)
        {
                if (buf[i] == '\n')
                        pos += VGA_WIDTH - ((i + pos) % VGA_WIDTH) - 1;
                else
                        fb_write_cell((i+pos)*2, buf[i], FB_DARK_GREY, FB_GREEN);
        }
}

/** serial_write:
        *  Writes a character buffer to the a serial port
        *
        *  @param *buf The pointer to the buffer that is going to be written
        *  @param len  The length of the buffer
        */
void serial_write(const char *buf, unsigned int len)
{
        for (unsigned int i = 0; i < len; i++)
                serial_write_char(SERIAL_COM1_BASE, buf[i]);
}

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
int cprint(unsigned int pos, const char *buf, unsigned char mode)
{
        switch(mode)
        {
                case 0:
                        fb_write(pos, buf, cstrlen(buf));
                        break;
                case 1:
                        serial_write(buf, cstrlen(buf));
                        break;
                case 2:
                        fb_write(pos, buf, cstrlen(buf));
                        serial_write(buf, cstrlen(buf));
                        break;
                default:
                        return 1;
        }

        return 0;
}


/* kernel main routine */
void kmain()
{
        fb_clear();

        char *greet = "Hello World!!!";
        char *something = "Happy birthday to you \nDaniel\n!!!";

        cprint(0, greet, 2);

        cprint(cstrlen(greet), something, 2);

}


