global outb
global inb
global load_gdt
global reload_segments
global load_idt

global pause

                                ; make the labels visible outside this file


 
                                ; outb - send a byte to an I/O port
                                ; stack: [esp + 8] the data byte
                                ;        [esp + 4] the I/O port
                                ;        [esp    ] return address
outb:
        mov al, [esp + 8]       ; move the data to be sent into the al register
        mov dx, [esp + 4]       ; move the address of the I/O port into the dx register
        out dx, al              ; send the data to the I/O port
        ret                     ; return to the calling function

 
                                ; inb - returns a byte from the given I/O port
                                ; stack: [esp + 4] The address of the I/O port
                                ;        [esp    ] The return address
inb:
        mov dx, [esp + 4]       ; move the address of the I/O port to the dx register
        in  al, dx              ; read a byte from the I/O port and store it in the al register
        ret                     ; return the read byte

 
                                ; load_gdt - loads the gdt table from a defined struct
                                ; stack: [esp + 4] The address of the gdt struct
                                ;        [esp    ] The return address

load_gdt:
        mov eax, [esp + 4]      ; move the address to the gdt struct to the eax register
        lgdt [eax]              ; load the gdt table defined in the struct
        ret

 
                                ; reload_segments - reloads the segment registers

reload_segments:                ; reload the CS register containing code selector
        jmp 0x08:.reload_cs     ; 0x08, index 1 in the GDT which is the kernel code segment
.reload_cs:
        cli
        mov ax, 0x10            ; 0x10, index 2 in the GDT which is the kernel data segment
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        sti
        ret

load_idt:
        mov eax, [esp + 4]      ; load the address of the IDT passed
        lidt [eax]              ; load the Interrupt Descriptor Table
        ret


pause:
        cli
        hlt
        jmp $
