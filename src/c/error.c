#include "error.h"
#include "kmain.h"
#include "interrupts.h"


/* panic:
 *      Halts the system and dumps the current states of the system
 *      depending on what type of panic it is
 *
 *      @param 
 */
void panic(panic_source_t reason, interrupt_stack_state_t *info)
{
        __asm__ __volatile__ (
                "cli" : : : "memory"
        );

        fb_clear();

        kprintf("============================================================\n\n");
        kprintf("                         KERNEL PANIC\n\n");
        kprintf("============================================================\n\n");

        switch (reason) {
                case PANIC_DIVIDE_BY_ZERO:
                        kprintf("DIVISION ERROR OCCURRED\n");
                        break;
                case PANIC_INVALID_OPCODE:
                        kprintf("INVALID OPCODE\n");
                        break;
                case PANIC_DOUBLE_FAULT:
                        kprintf("DOUBLE FAULT\n");
                        break;
                case PANIC_GENERAL_PROTECTION_FAULT:
                        kprintf("GENERAL PROTECTION FAULT\n");
                        break;
                case PANIC_PAGE_FAULT:
                        break;
                case PANIC_OUT_OF_MEMORY:
                        break;
                case PANIC_UNREACHABLE:
                        break;

                default:
                        break;
        }

        kprintf("\n");

        kprintf("SYSTEM INFORMATION:\n");
        kprintf("EFLAGS:     %x\n", info->eflags);
        kprintf("CS:         %x\n", info->cs);
        kprintf("EIP:        %x\n", info->eip);
        kprintf("INTERRUPT:  %x\n", info->interrupt);
        kprintf("ERROR CODE: %x\n", info->error_code);
        kprintf("EAX:        %x\n", info->eax);
        kprintf("EBX:        %x\n", info->ebx);
        kprintf("ECX:        %x\n", info->ecx);
        kprintf("EDX:        %x\n", info->edx);
        kprintf("ESI:        %x\n", info->esi);
        kprintf("EDI:        %x\n", info->edi);
        kprintf("EBP:        %x\n", info->ebp);
        kprintf("ESP:        %x\n", info->esp_dummy);

        pause();
}
