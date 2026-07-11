#include "keyboard.h"
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

#include "kmain.h"
#include "io.h"
#include "mem_init.h"
#include "interrupts.h"

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
void fb_write_cell(uint32_t i, char c, uint8_t fg, uint8_t bg)
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
        const int size = 80 * 25 * 2;
        char *fb = (char *) FRAME_BUFFER;

        for (int i = 0; i < size; i++)
                fb[i] = 0x00;
}

/** fb_write:
 *  Writes a character buffer to the frame buffer
 *
 *  @param *row  The pointer to the row number that is going to be written at
 *  @param *col  The pointer to the column number that is going to be written at
 *  @param *buf The pointer to the buffer that is going to be written out
 *  @param len  The length of the buffer
 */
void fb_write(uint32_t *row, uint32_t *col, const char *buf, unsigned int len)
{
        for (unsigned int i = 0; i < len; i++)
        {       
                if (buf[i] == '\n'){
                        (*row)++;
                        (*col) = 0;
                } else {
                        fb_write_cell(((*row) * VGA_WIDTH + (*col))*2, buf[i], FB_DARK_GREY, FB_GREEN);

                        if (*col+1 >= VGA_WIDTH)
                                (*row)++;
                        else
                                (*col)++;
                }
        }
        fb_move_cursor(((*row) * VGA_WIDTH + (*col))*2);
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

/** kprintf:
 *  Writes a buffer to a desination depending on the selected mode
 *
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
int kprintf(uint32_t *row, uint32_t *col, const char *buf, uint8_t mode)
{
        switch(mode)
        {
                case 0:
                        fb_write(row, col, buf, cstrlen(buf));
                        break;
                case 1:
                        serial_write(buf, cstrlen(buf));
                        break;
                case 2:
                        fb_write(row, col, buf, cstrlen(buf));
                        serial_write(buf, cstrlen(buf));
                        break;
                default:
                        return 1;
        }

        return 0;
}


/* kernel main routine */
void kmain(void)
{
       
        fb_clear();
        uint32_t row = 0;
        uint32_t col = 0;

        char *greet = "Hello World!!!\n";
        char *something = "Testing for print function\nNew line\nAnother new line\n";

        kprintf(&row, &col, greet, 2);

        kprintf(&row, &col, something, 2);

        protected_mode_gdt();
        kprintf(&row, &col, "Protected mode segmentation is set and loaded\n", 2);

        create_idt();
        kprintf(&row, &col, "IDT loaded\n", 2);

        init_ps2_kbd();
        kprintf(&row, &col, "Keyboard event queue initialised\n", 2);

        pic_init();
        kprintf(&row, &col, "PIC initialised, keyboard interrupt unmasked\n", 2);

        keycode_t key;
        char *message;
        while (1){

                key = key_queue_get_event();
                if (key != NO_KEY){
                        message = (char *)handle_keycode(key);
                        kprintf(&row, &col, message, 2);
                }
        }
        pause();
}


