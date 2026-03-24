#include "interrupts.h"
#include "kmain.h"

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

/* load_idt:
        *  loads the Interrupt Descriptor Table
        *
        *  @param *idt  The struct pointer to the IDT
        */
void load_idt(struct idt_ptr *idt);

/* create_idt:
        *  creates a interrupt descriptor table and loads it
        */
void create_idt()
{
        static struct idt_entry idt[256];
        
        struct idt_ptr idt_address;
        idt_address.address = (unsigned int)idt;
        idt_address.size = sizeof(idt) - 1;

        load_idt(&idt_address);
}

void interrupt_handler(struct interrupt_stack_state *stack){
        stack += 1;
        pause();        
}

