section .text

global x86_load_tss

x86_load_tss:
    mov ax, 0x30
    ltr ax
    ret