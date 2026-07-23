#include <stdarg.h>
#include <stdint.h>
#define FRAME_BUFFER            0x000B8000

/* COM1 base port */
#define SERIAL_COM1_BASE        0x3F8      

#define VGA_WIDTH               80
#define VGA_SIZE                VGA_WIDTH * 25 * 2

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

#include "multiboot.h"
#include "kmain.h"
#include "io.h"
#include "mem_init.h"
#include "interrupts.h"
#include "keyboard.h"

typedef void (*call_module_t)(void);

/* Tracker for the current position on the frame buffer */
static uint8_t row = 0;
static uint8_t col = 0;

/* cstrlen:
 *      Count the length of a string buffer
 *
 *      @param *str  The buffer to be counted
 *      @return  The length of the buffer passed
 */
uint32_t cstrlen(const char *str)
{
        unsigned int len = 0;

        while (str[len] != '\0')
                len++;

        return len;
}

/* reverse_string:
 *      Reverses a string for the first len bytes
 *
 *      @param *str  The string to be reversed
 *              len  Numbers of bytes from the string to be reversed
 *
 */
void reverse_string(char *str, uint32_t len)
{
        if (!str || len < 2)
                return;

        uint32_t i = 0;
        uint32_t j = len - 1;

        while (i < j)
        {
                char tmp = str[i];
                str[i] = str[j];
                str[j] = tmp;

                i++;
                j--;
        }
}

/* fb_screen_up:
 *      Moves the entire frame buffer up by 1 row
 */
void fb_screen_up(void){

        char *fb = (char *) FRAME_BUFFER;

        for (uint32_t i = 0; i < ((VGA_SIZE - VGA_WIDTH*2)); i++)
                fb[i] = fb[i+(VGA_WIDTH*2)];

        for (uint16_t i = VGA_SIZE - (VGA_WIDTH * 2); i < VGA_SIZE; i++)
                fb[i] = 0;

}

/* fb_write_cell:
 *      Writes a character with the given foreground and background to position i
 *      in the framebuffer.
 *
 *      @param c  The character
 *      @param bg The background color
 *      @param fg The foreground color
 */
void fb_write_cell(char c, uint8_t bg, uint8_t fg)
{
        char *fb = (char *) FRAME_BUFFER;

        uint32_t pos = ((row * VGA_WIDTH) + col) * 2;

        if (c != '\n'){
                fb[pos] = c;
                fb[pos+1] = (((bg & 0x0f) << 4) | (fg & 0x0f));
        }

        if (col + 1 >= VGA_WIDTH || c == '\n'){
                if (row + 1 >= 25){
                        fb_screen_up();
                        row--;
                }

                row++;
                col = 0;
        } else
                col++;

        fb_move_cursor((row * VGA_WIDTH) + col);
}

/* fb_clear:
 *      Clears the frame buffer (make blank screen)
 */
void fb_clear(void)
{
        char *fb = (char *) FRAME_BUFFER;

        for (uint32_t i = 0; i < VGA_SIZE; i++)
                fb[i] = 0x00;

        row = 0;
        col = 0;
        fb_move_cursor(0);
}

/* kputchar:
 *      prints one character
 *      
 *      @param c  The character to be printed
 */
void kputchar(char c)
{
        fb_write_cell(c, FB_BLACK, FB_GREEN);
        serial_write_char(SERIAL_COM1_BASE, c);
}

/* kprint:
 *      Writes a buffer to a desination depending on the selected mode
 *
 *      @param *buf  The buffer that is going to be written out
 */
void kprint(const char *buf)
{
        if (!buf)
                buf = "(null)";

        while(*buf)
                kputchar(*buf++);

}

/* Number characters look up tables */
static const char numbers_lower[] = "0123456789abcdef";
static const char numbers_upper[] = "0123456789ABCDEF";

/* int_to_str:
 *      Converts integer into their equivalent in string
 *      !!! When base == 16, num expects non-negative values,
 *          otherwise unexpected behaviours may occur.
 *      
 *      @param num      The integer to convert
 *             base     The base of the target converted string
 *             capital  Whether the letters should be in upper case,
 *                      not used when base <= 10
 *                        1 - Upper case
 *                        0 - Lower case
 */
void int_to_str(int32_t num, char *buffer, uint8_t base, uint8_t capital)
{
        uint8_t count_digit = 0;
        uint8_t negative = 0;

        if (base != 10 && base != 16){
                return;
        }

        if (num == 0){
                buffer[0] = '0';
                buffer[1] = '\0';
                return;
        }

        if (num < 0) {
                negative = 1;
                num = -num;
        }

        const char *numbers = capital ? numbers_upper : numbers_lower;

        while (num)
        {

                buffer[count_digit] = numbers[num % base];

                count_digit++;
                num = (int32_t)(num / base);
        }

        if (negative && base == 10){
                buffer[count_digit] = '-';
                count_digit++;
        }

        reverse_string(buffer, count_digit);

        buffer[count_digit] = '\0';
}

/* kprintf:
 *      Prints to the VGA frame buffer and serial port COM1 with format
 *
 *      @param *format  A string describing the format of the target to be print
 *              ...     Additional arguments to fill in placeholders
 */
void kprintf(const char *format, ...)
{
        va_list args;

        va_start(args, format);


        while (*format)
        {

                if (*format != '%'){
                        kputchar(*format);
                        format++;
                        continue;
                }

                format++;
                if (*format == '\0')
                        break;

                char buffer[12];
                char *str;
                int32_t num;

                switch (*format)
                {
                        case 'c':
                                kputchar((char)va_arg(args, int));
                                break;

                        case 's':
                                str = va_arg(args, char*);
                                kprint(str);
                                break;

                        case 'd':
                                num = va_arg(args, int);
                                int_to_str(num, buffer, 10, 0);
                                kprint(buffer);
                                break;

                        case 'x':
                                num = va_arg(args, int);
                                int_to_str(num, buffer, 16, 0);
                                kprint("0x");
                                kprint(buffer);
                                break;

                        case 'X':
                                num = va_arg(args, int);
                                int_to_str(num, buffer, 16, 1);
                                kprint("0x");
                                kprint(buffer);
                                break;

                        default:
                                kputchar('%');
                                kputchar(*format);
                                break;
                }

                format++;
        }

        va_end(args);
}


/* kernel main routine */
void kmain(uint32_t ebx)
{
        multiboot_info_t *mbinfo = (multiboot_info_t *)ebx;

        if (mbinfo->flags & MULTIBOOT_INFO_MODS && mbinfo->mods_count == 1){
                multiboot_module_t *module = (multiboot_module_t *)mbinfo->mods_addr;
                kprintf("Multiboot module address loaded\n");
                kprintf("Loading module\n");
                call_module_t start_program = (call_module_t)(module[0].mod_start);
                kprintf("module start: %x\n", module[0].mod_start);
                start_program();
        }

       
        fb_clear();

        const char *greet = "Hello World!!!\n";

        kprintf("%s", greet);

        kprintf("Testing for print function\nNew line\n");

        kprintf("This should print a number %d\n", 53);

        kprintf("This should print a number in hex %x\n", 124);

        kprintf("This should print a number in upper case hex %X\n", 124);

        protected_mode_gdt();
        kprintf("Protected mode segmentation is set and loaded\n");

        create_idt();
        kprintf("IDT loaded\n");

        init_ps2_kbd();
        kprintf("Keyboard event queue initialised\n");

        pic_init();
        kprintf("PIC initialised, keyboard interrupt unmasked\n");

        keycode_t key;
        char *message;

        // #pragma GCC diagnostic push
        // #pragma GCC diagnostic ignored "-Wdiv-by-zero"
        // int aaa = 53/0;
        // #pragma GCC diagnostic pop
        //
        // kprintf("%d\n", aaa);
        //
        while (1)
        {
                key = key_queue_get_event();
                if (key != NO_KEY){
                        message = (char *)handle_keycode(key);
                        kprintf(message);
                }
        }
}


