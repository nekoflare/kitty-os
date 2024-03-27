bits 64

section .text
global x86_load_gdt

;; Params:
;;; rdi - gdtr_64* gdt_ptr
x86_load_gdt:
    lgdt [rdi]
x86_reload_segments:
    push 0x08
    lea rax, [rel .x86_reload_cs]
    push rax
    retfq
.x86_reload_cs:
    mov ax, 0x8 + 0x8
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax

    ret