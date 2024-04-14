bits 64

section .text

global KeASMReloadSegments
KeASMReloadSegments:
    lgdt [rdi]

    ; Reload segments
    push 0x8
    lea rax, [rel .reload_CS]
    push rax
    retfq
.reload_CS:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ret