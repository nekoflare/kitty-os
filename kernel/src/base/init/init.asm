bits 64

section .text

extern KernelMain
global _start

_start:
    ; Check for SSE
    mov rax, 0x1
    cpuid
    test edx, 1 << 25
    jz .NoSSE

    ; SSE exists.
    mov rax, cr0
    and ax, 0xFFFB
    or rax, 0x2
    mov cr0, rax
    mov rax, cr4
    or rax, 3 << 9
    mov cr4, rax

    call KernelMain
.NoSSE:
    cli
    hlt
    jmp $