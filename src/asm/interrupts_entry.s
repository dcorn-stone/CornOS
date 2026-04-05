extern interrupt_handler

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
        push dword 0            ; push 0 as error code for interrupts with no error code
        push dword %1           ; push the interrupt number
        jmp generic_handler     ; transfer to generic interrupt handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
        push dword %1
        jmp generic_handler
%endmacro

generic_handler:
        ; save all general purpose registers
        pushad

        ; pass the pointer to the struct to the C function
        push esp

        ; call the C interrupt handler
        call interrupt_handler

        ; clean up the esp value poped onto the stack (skip the struct pointer)
        add esp, 4

        ; clean up stack
        popad

        ; restore the esp
        add esp, 8
        
        ; return to interrupted program
        iret

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler 8
no_error_code_interrupt_handler 9
error_code_interrupt_handler 10
error_code_interrupt_handler 11
error_code_interrupt_handler 12
error_code_interrupt_handler 13
error_code_interrupt_handler 14
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
error_code_interrupt_handler 17
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
error_code_interrupt_handler 21
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
error_code_interrupt_handler 29
error_code_interrupt_handler 30
no_error_code_interrupt_handler 31

no_error_code_interrupt_handler 33
