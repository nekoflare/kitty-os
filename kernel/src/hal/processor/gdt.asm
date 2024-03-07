section .text
global load_gdt

load_gdt:
    lgdt [rdi]
reload_segments:
    push 0x08
    lea rax, [rel .reload_CS]
    push rax
    retfq
.reload_CS:
    mov ax, 0x8 + 0x8
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax

    ret

