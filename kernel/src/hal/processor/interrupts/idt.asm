bits 64
section .text

extern interrupt_handler
extern interrupt_handler
global idt_load

idt_load:
    lidt [rdi]
    sti
    ret

%macro ISR_NOERRORCODE 1

global ISR%1:
ISR%1:
    push qword 0    ; dummy error code
    push qword %1   ; interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERRORCODE 1
global ISR%1:
ISR%1:
    push %1
    jmp isr_common
%endmacro

%include "src/hal/processor/interrupts/isrs.inc"

isr_common:
    swapgs

    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, cs
    and rax, 0xffff
    push rax

    mov ax, ds
    and rax, 0xffff
    push rax
    
    mov ax, ss
    and rax, 0xffff
    push rax

    mov ax, es
    and rax, 0xffff
    push rax
    
    mov ax, fs
    and rax, 0xffff
    push rax
    
    mov ax, gs
    and rax, 0xffff
    push rax

    cld

    call interrupt_handler

    add rsp, 6 * 8

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    swapgs
    add rsp, 16
    iretq