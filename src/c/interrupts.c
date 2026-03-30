#include "interrupts.h"
#include "io.h"
#include "kmain.h"

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

/* idt_ptr: Struct containing the base address and the limit of a Intterupt Descriptor Table */
struct idt_ptr{
        unsigned short size;  // The limit/size of the IDT    0-15 bit
        unsigned int address; // The base address of the IDT  16-31 bit
} __attribute__((packed));

/* idt_entry:
 * struct of a single entry in the interrupt descriptor table
 */
struct idt_entry{
        unsigned short offset_low;              // offset bits 0-15
        unsigned short segment_selector;        // a code segment selector in GDT or LDT
        unsigned char zero;                     // zero and reserved bits
        unsigned char type_attributes;          // gate type, DPL, and P fields
        unsigned short offset_high;             // offset bits 16-31
} __attribute__((packed));

/* interrupt_stack_state
 * struct to save the values of general purpose registers and values
 * pushed by the CPU cause by an interrupt
 */
struct interrupt_stack_state{
        unsigned int eax;
        unsigned int ebx;
        unsigned int ecx;
        unsigned int edx;
        unsigned int esi;
        unsigned int edi;
        unsigned int ebp;

        unsigned int interrupt;

        unsigned int error_code;
        unsigned int eip;
        unsigned int cs;
        unsigned int eflags;
} __attribute__((packed));

/* set_idt_entry:
 *  assigns the handler of a interrupt in the IDT
 *
 *  @param *entry  An entry in the IDT that is getting set
 *  @param handler The handler for that specific interrupt
 */
void set_idt_entry(struct idt_entry *entry, unsigned int handler)
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

/* read_scan_code;
 *  Read the scan code from the keyboard's data port
 *
 *  @return  The scan code from the keyboard
 */
unsigned char read_scan_code()
{
        return inb(KBD_DATA_PORT);
}

void irq_1_keyboard_interrupt_handler(){
        fb_clear();
        char scan_code = read_scan_code();
        cprint(0, &scan_code, 2);
        
}

/* create_idt:
 *  creates a interrupt descriptor table and loads it
 */
void create_idt()
{
        static struct idt_entry idt[256];

        set_idt_entry(&idt[0], (unsigned int)interrupt_handler_0);
        set_idt_entry(&idt[1], (unsigned int)interrupt_handler_1);
        set_idt_entry(&idt[2], (unsigned int)interrupt_handler_2);
        set_idt_entry(&idt[3], (unsigned int)interrupt_handler_3);
        set_idt_entry(&idt[4], (unsigned int)interrupt_handler_4);
        set_idt_entry(&idt[5], (unsigned int)interrupt_handler_5);
        set_idt_entry(&idt[6], (unsigned int)interrupt_handler_6);
        set_idt_entry(&idt[7], (unsigned int)interrupt_handler_7);
        set_idt_entry(&idt[8], (unsigned int)interrupt_handler_8);
        set_idt_entry(&idt[9], (unsigned int)interrupt_handler_9);
        set_idt_entry(&idt[10], (unsigned int)interrupt_handler_10);
        set_idt_entry(&idt[11], (unsigned int)interrupt_handler_11);
        set_idt_entry(&idt[12], (unsigned int)interrupt_handler_12);
        set_idt_entry(&idt[13], (unsigned int)interrupt_handler_13);
        set_idt_entry(&idt[14], (unsigned int)interrupt_handler_14);
        set_idt_entry(&idt[15], (unsigned int)interrupt_handler_15);
        set_idt_entry(&idt[16], (unsigned int)interrupt_handler_16);
        set_idt_entry(&idt[17], (unsigned int)interrupt_handler_17);
        set_idt_entry(&idt[18], (unsigned int)interrupt_handler_18);
        set_idt_entry(&idt[19], (unsigned int)interrupt_handler_19);
        set_idt_entry(&idt[20], (unsigned int)interrupt_handler_20);
        set_idt_entry(&idt[21], (unsigned int)interrupt_handler_21);
        set_idt_entry(&idt[22], (unsigned int)interrupt_handler_22);
        set_idt_entry(&idt[23], (unsigned int)interrupt_handler_23);
        set_idt_entry(&idt[24], (unsigned int)interrupt_handler_24);
        set_idt_entry(&idt[25], (unsigned int)interrupt_handler_25);
        set_idt_entry(&idt[26], (unsigned int)interrupt_handler_26);
        set_idt_entry(&idt[27], (unsigned int)interrupt_handler_27);
        set_idt_entry(&idt[28], (unsigned int)interrupt_handler_28);
        set_idt_entry(&idt[29], (unsigned int)interrupt_handler_29);
        set_idt_entry(&idt[30], (unsigned int)interrupt_handler_30);
        set_idt_entry(&idt[31], (unsigned int)interrupt_handler_31);
        set_idt_entry(&idt[33], (unsigned int)irq_1_keyboard_interrupt_handler);
        
        struct idt_ptr idt_address;
        idt_address.address = (unsigned int)idt;
        idt_address.size = sizeof(idt) - 1;

        load_idt(&idt_address);
}


/*  pic_sendEOI:
 *  Tells the PIC that the IRQ is handled and the process is finished
 */
void pic_sendEOI(unsigned char irq)
{
        if(irq >= 8)
                outb(PIC2_COMMAND, PIC_EOI);

        outb(PIC1_COMMAND, PIC_EOI);
}

/*  pic_remap:
 *  Reinitialise the PICs with a new offset
 *
 *  @param offset1  The new vector offset for the master PIC
 *  @param offset2  The new vector offset for the slaze PIC
 */
void pic_remap(unsigned int offest1, unsigned int offset2)
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
void irq_set_mask(unsigned char irq_line)
{
        unsigned short port;
        unsigned char value;

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
 *  @param irq_line  The IRQ desired to be unmasked
 */
void irq_clear_mask(unsigned char irq_line)
{
        unsigned short port;
        unsigned char value;

        if (irq_line < 8){
                port = PIC1_DATA;
        } else {
                port = PIC2_DATA;
                irq_line -= 8;
        }

        value = inb(port) & ~(1 << irq_line);

        outb(port, value);
}

/*  interrupt_handler:
 *  handles the first 32 interrupts/exceptions generated by the CPU in protected mode
 *
 *  @param  *stack  The state of registers and interrupt number, error code,
 *  - the EIP register, the cs register and eflags pushed onto the stack
 *  - for safe return
 */
void interrupt_handler(struct interrupt_stack_state *stack){
        stack += 1;
        pause();        
}


void pic_init()
{
        pic_remap(PIC1_NEW_OFFSET, PIC2_NEW_OFFSET);
        pic_disable();
        irq_clear_mask(1); // Unmask keyboard interrupt
}

