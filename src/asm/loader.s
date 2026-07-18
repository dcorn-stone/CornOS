MAGIC_NUMBER    equ 0x1BADB002                                     ; define the magic number constant
ALIGN_MODULES   equ 0x00000001                                     ; tell GRUB to align modules

; (magic number + checksum + all options should equal 0)
CHECKSUM        equ -(MAGIC_NUMBER + ALIGN_MODULES)                


KERNEL_STACK_SIZE equ 4096                                      ; size of stack in bytes

section .bss
align 4                                                         ; align at 4 bytes
kernel_stack:                                                   ; label points to beginning of memory
        resb KERNEL_STACK_SIZE                                  ; reserve stack for the kernel

section .multiboot
align 4                                                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER                                         ; write the magic number
        dd ALIGN_MODULES                                        ; write the align modules instruction
        dd CHECKSUM                                             ; write the checksum

section .text                                                   ; start of the text (code) section

global loader                                                   ; the entry symbol for ELF
extern kmain

loader:                                                         ; the loader label (defined as entry point in linker script)
        mov eax, 0xCAFEBABE                                     ; place the number 0xCAFEBABE in the register eax
        mov esp, kernel_stack + KERNEL_STACK_SIZE               ; points to the end of the reserved memory section (the stack)
                                                                ; (stack starts from higher address and grows to lower)
        push ebx                                                ; Grub stores a pointer to struct in ebx of the loaded module
        call kmain                                              ; transfer control to kernel in C


.loop:
        jmp .loop                                               ; loop forever
