bits 64

section .text

extern interrupt_manager

%macro ISR_TEMPLATE 1
    global isr%1
    isr%1:
        push rax
        push rcx
        push rdx
        push rbx
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

        push qword %1

        call interrupt_manager

        pop r15 ;; this is ignored anyways. useless
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
        pop rbx
        pop rdx
        pop rcx
        pop rax
        iretq
%endmacro

%assign i 0
%rep 256
    ISR_TEMPLATE i
    %assign i i+1
%endrep