section .text

global x86_load_tss

x86_load_tss:
    mov ax, 0x28
    ltr ax
    ret