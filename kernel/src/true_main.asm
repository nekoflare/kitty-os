section .data


section .text

extern kernel_main
global _start

%define SSE_BIT 1 << 25

enable_sse:
    ;; Set CR4.OSFXSR
    mov rax, cr4
    or rax, (1 << 9)
    mov cr4, rax

    ;; Disable CR0.EM
    mov rax, cr0
    and rax, ~(1 << 2)
    mov cr0, rax

    ;; Enable CR0.MP
    mov rax, cr0
    or rax, (1 << 1)
    mov cr0, rax

    ;; Clear CR0.TS 
    mov rax, cr0
    and rax, ~(1 << 3)
    mov cr0, rax

    ret


_start:
    call enable_sse

        
    ;; enable cache 
    mov rax, cr0
    or rax, 1 << 30
    mov cr0, rax

    ;; Enable PSE (Page Size Extensions)
    mov rax, cr4
    or rax, 1 << 4
    mov cr4, rax

    ;; Enable VMXE (Virtual Machine Extensions Enable)
    ;;mov rax, cr4
    ;;or rax, 1 << 13
    ;;mov cr4, rax

    ;; Enable SMXE (Safer Mode Extensions Enable)
    ;;mov rax, cr4
    ;;or rax, 1 << 14
    ;;mov cr4, rax

    ;; Enable FSGSBASE (Enables rdfsbase rdgsbase wrfsbase and wrgsbase)
    mov rax, cr4
    or rax, 1 << 16
    mov cr4, rax

    ;; Enable UMIP (disable SGDT, SIDT, SLDT, SMSW and STR instructions for RING 3)
    ;;mov rax, cr4
    ;;or rax, 1 << 11
    ;;mov cr4, rax

    ;; Enable execute disable and PAE
    mov rax, cr4
    or rax, 1 << 5
    mov cr4, rax

    ;; Enable syscall extensions.
    mov rcx, 0xC0000080 ;; EFER MSR
    rdmsr               ;; edx : eax has the contents.
    or rax, 1           ;; enable syscall extensions by setting bit 0.

    ;; Write MSR back
    wrmsr

    ;; Execute disable part.
    mov rcx, 0xC000008
    rdmsr
    or rax, 1 << 11 

    ;; Write MSR back.
    wrmsr

    call kernel_main

    .hang:
        cli
        hlt
        jmp .hang

section .data