#include "interrupts.h"
#include "io.h"
#include "kmain.h"
#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define PIC1_NEW_OFFSET 0x20
#define PIC2_NEW_OFFSET 0x28

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_IRQ 2

#define KBD_DATA_PORT   0x60

extern void interrupt_handler_0();
extern void interrupt_handler_1();
extern void interrupt_handler_2();
extern void interrupt_handler_3();
extern void interrupt_handler_4();
extern void interrupt_handler_5();
extern void interrupt_handler_6();
extern void interrupt_handler_7();
extern void interrupt_handler_8();
extern void interrupt_handler_9();
extern void interrupt_handler_10();
extern void interrupt_handler_11();
extern void interrupt_handler_12();
extern void interrupt_handler_13();
extern void interrupt_handler_14();
extern void interrupt_handler_15();
extern void interrupt_handler_16();
extern void interrupt_handler_17();
extern void interrupt_handler_18();
extern void interrupt_handler_19();
extern void interrupt_handler_20();
extern void interrupt_handler_21();
extern void interrupt_handler_22();
extern void interrupt_handler_23();
extern void interrupt_handler_24();
extern void interrupt_handler_25();
extern void interrupt_handler_26();
extern void interrupt_handler_27();
extern void interrupt_handler_28();
extern void interrupt_handler_29();
extern void interrupt_handler_30();
extern void interrupt_handler_31();

extern void interrupt_handler_33();

/* idt_ptr: Struct containing the base address and the limit of a Intterupt Descriptor Table */
struct idt_ptr{
        uint16_t size;          // The limit/size of the IDT    0-15 bit
        uint32_t address;       // The base address of the IDT  16-31 bit
} __attribute__((packed));

/* idt_entry:
 * struct of a single entry in the interrupt descriptor table
 */
struct idt_entry{
        uint16_t offset_low;              // offset bits 0-15
        uint16_t segment_selector;        // a code segment selector in GDT or LDT
        uint8_t zero;                     // zero and reserved bits
        uint8_t type_attributes;          // gate type, DPL, and P fields
        uint16_t offset_high;             // offset bits 16-31
} __attribute__((packed));

/* interrupt_stack_state:
 * struct to save the values of all general purpose registers and values
 * pushed by the CPU caused by an interrupt
 */
struct interrupt_stack_state{
        uint32_t edi;
        uint32_t esi;
        uint32_t ebp;
        uint32_t esp_dummy;
        uint32_t ebx;
        uint32_t edx;
        uint32_t ecx;
        uint32_t eax;

        uint32_t interrupt;

        uint32_t error_code;
        uint32_t eip;
        uint32_t cs;
        uint32_t eflags;
} __attribute__((packed));

/* set_idt_entry:
 *  assigns the handler of a interrupt in the IDT
 *
 *  @param *entry  An entry in the IDT that is getting set
 *  @param handler The handler for that specific interrupt
 */
void set_idt_entry(struct idt_entry *entry, uint32_t handler)
{
        entry->offset_low = handler & 0xffff;
        entry->segment_selector = 0x08; // kernel code segment
        entry->zero = 0;
        entry->type_attributes = 0x8e; // requires ring 0, 32 bit interrupt gate
        entry->offset_high = (handler >> 16) & 0xffff;
}

/* load_idt:
 *  loads the Interrupt Descriptor Table
 *
 *  @param *idt  The struct pointer to the IDT
 */
void load_idt(struct idt_ptr *idt);


/*  pic_sendEOI:
 *  Tells the PIC that the IRQ is handled and the process is finished
 */
void pic_sendEOI(uint8_t irq)
{
        if(irq >= 8)
                outb(PIC2_COMMAND, PIC_EOI);

        outb(PIC1_COMMAND, PIC_EOI);
}

/* read_scan_code;
 *  Read the scan code from the keyboard's data port
 *
 *  @return  The scan code from the keyboard
 */
char read_scan_code()
{
        return inb(KBD_DATA_PORT);
}

void irq_1_keyboard_interrupt_handler(void)
{
        fb_clear();
        uint32_t row = 0;
        uint32_t col = 0;
        char scan_code = read_scan_code();
        kprintf(&row, &col, &scan_code, 2);
        io_wait();
}

/*  pic_remap:
 *  Reinitialise the PICs with a new offset
 *
 *  @param offset1  The new vector offset for the master PIC
 *  @param offset2  The new vector offset for the slaze PIC
 */
void pic_remap(uint32_t offest1, uint32_t offset2)
{
        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();

        outb(PIC1_DATA, offest1);               // ICW2: Master PIC vector offset
        io_wait();
        outb(PIC2_DATA, offset2);               // ICW2: Slave PIC vector offset
        io_wait();

        outb(PIC1_DATA, 1 << CASCADE_IRQ);      // ICW3: Tells the master PIC that the slave PIC is at IRQ2
        io_wait();
        outb(PIC2_DATA, 2);                     // ICW3: Tells the slave PIC its cascade identity (0000 0010)
        io_wait();

        outb(PIC1_DATA, ICW4_8086);
        io_wait();
        outb(PIC2_DATA, ICW4_8086);
        io_wait();

        // Unmask both PICs
        outb(PIC1_DATA, 0);
        outb(PIC2_DATA, 0);
}

/*  pic_disable:
 *  Masks all the interrupts from the PIC by setting every bit
 *  - in the Interrupt Mask Register
 */
void pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

/*  irq_set_mask:
 *  Masks an IRQ by setting the coresponding Interrupt Mask Register bit
 *
 *  @param irq_line  The IRQ desired to be masked
 */
void irq_set_mask(uint8_t irq_line)
{
        uint16_t port;
        uint8_t value;

        if (irq_line < 8){
                port = PIC1_DATA;
        } else {
                port = PIC2_DATA;
                irq_line -= 8;
        }

        value = inb(port) | (1 << irq_line);

        outb(port, value);
}

/*  irq_clear_mask:
 *  Unmasks an IRQ by clearing the coresponding Interrupt Mask Register bit
 *
 *  @param irq_line  The target IRQ to be unmasked
 */
void irq_clear_mask(uint8_t irq_line)
{
        uint16_t port;
        uint8_t value;

        if (irq_line < 8){
                port = PIC1_DATA;
        } else {
                port = PIC2_DATA;
                irq_line -= 8;
        }

        value = inb(port) & ~(1 << irq_line);

        outb(port, value);
}

/* create_idt:
 *  creates an interrupt descriptor table and loads it
 */
void create_idt()
{
        static struct idt_entry idt[256];

        set_idt_entry(&idt[0], (uint32_t)interrupt_handler_0);
        set_idt_entry(&idt[1], (uint32_t)interrupt_handler_1);
        set_idt_entry(&idt[2], (uint32_t)interrupt_handler_2);
        set_idt_entry(&idt[3], (uint32_t)interrupt_handler_3);
        set_idt_entry(&idt[4], (uint32_t)interrupt_handler_4);
        set_idt_entry(&idt[5], (uint32_t)interrupt_handler_5);
        set_idt_entry(&idt[6], (uint32_t)interrupt_handler_6);
        set_idt_entry(&idt[7], (uint32_t)interrupt_handler_7);
        set_idt_entry(&idt[8], (uint32_t)interrupt_handler_8);
        set_idt_entry(&idt[9], (uint32_t)interrupt_handler_9);
        set_idt_entry(&idt[10], (uint32_t)interrupt_handler_10);
        set_idt_entry(&idt[11], (uint32_t)interrupt_handler_11);
        set_idt_entry(&idt[12], (uint32_t)interrupt_handler_12);
        set_idt_entry(&idt[13], (uint32_t)interrupt_handler_13);
        set_idt_entry(&idt[14], (uint32_t)interrupt_handler_14);
        set_idt_entry(&idt[15], (uint32_t)interrupt_handler_15);
        set_idt_entry(&idt[16], (uint32_t)interrupt_handler_16);
        set_idt_entry(&idt[17], (uint32_t)interrupt_handler_17);
        set_idt_entry(&idt[18], (uint32_t)interrupt_handler_18);
        set_idt_entry(&idt[19], (uint32_t)interrupt_handler_19);
        set_idt_entry(&idt[20], (uint32_t)interrupt_handler_20);
        set_idt_entry(&idt[21], (uint32_t)interrupt_handler_21);
        set_idt_entry(&idt[22], (uint32_t)interrupt_handler_22);
        set_idt_entry(&idt[23], (uint32_t)interrupt_handler_23);
        set_idt_entry(&idt[24], (uint32_t)interrupt_handler_24);
        set_idt_entry(&idt[25], (uint32_t)interrupt_handler_25);
        set_idt_entry(&idt[26], (uint32_t)interrupt_handler_26);
        set_idt_entry(&idt[27], (uint32_t)interrupt_handler_27);
        set_idt_entry(&idt[28], (uint32_t)interrupt_handler_28);
        set_idt_entry(&idt[29], (uint32_t)interrupt_handler_29);
        set_idt_entry(&idt[30], (uint32_t)interrupt_handler_30);
        set_idt_entry(&idt[31], (uint32_t)interrupt_handler_31);

        set_idt_entry(&idt[33], (uint32_t)interrupt_handler_33);
        
        struct idt_ptr idt_address;
        idt_address.address = (uint32_t)idt;
        idt_address.size = sizeof(idt) - 1;

        load_idt(&idt_address);
}

/*  interrupt_handler:
 *  handles the first 32 interrupts/exceptions generated by the CPU in protected mode
 *
 *  @param  *stack  The state of registers and interrupt number, error code,
 *  - the EIP register, the cs register and eflags pushed onto the stack
 *  - for safe return
 */
void interrupt_handler(struct interrupt_stack_state *frame){
        switch (frame->interrupt) {
                case 0:
                        break;
                case 1:
                        break;
                case 2:
                        break;
                case 3:
                        break;
                case 4:
                        break;
                case 5:
                        break;
                case 6:
                        break;
                case 7:
                        break;
                case 8:
                        break;
                case 9:
                        break;
                case 10:
                        break;
                case 11:
                        break;
                case 12:
                        break;
                case 13:
                        break;
                case 14:
                        break;
                case 15:
                        break;
                case 16:
                        break;
                case 17:
                        break;
                case 18:
                        break;
                case 19:
                        break;
                case 20:
                        break;
                case 21:
                        break;
                case 22:
                        break;
                case 23:
                        break;
                case 24:
                        break;
                case 33:
                        irq_1_keyboard_interrupt_handler();
                        break; 
 
 
        }
        if (frame->interrupt >= PIC1_NEW_OFFSET && frame->interrupt <= PIC2_NEW_OFFSET){
                pic_sendEOI(frame->interrupt - PIC1_NEW_OFFSET);
        }
}

void pic_init()
{
        pic_remap(PIC1_NEW_OFFSET, PIC2_NEW_OFFSET);
        pic_disable();
        irq_clear_mask(1); // Unmask keyboard interrupt
}

